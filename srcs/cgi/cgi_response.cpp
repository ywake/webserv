#include <netdb.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <utility>
#include <vector>

#include "cgi_response.hpp"
#include "debug.hpp"
#include "http_exceptions.hpp"
#include "stat.hpp"
#include "webserv_utils.hpp"
namespace
{
	Result<std::pair<std::string, std::string> >
	GetSocketAddress(const struct sockaddr_storage *addr)
	{
		socklen_t socklen   = sizeof(addr);
		char      ip[256]   = {};
		char      port[256] = {};

		if (getnameinfo(
				(struct sockaddr *)addr,
				socklen,
				ip,
				sizeof(ip),
				port,
				sizeof(port),
				NI_NUMERICHOST | NI_NUMERICSERV
			) != 0) {
			return Error("getnameinfo error");
		}
		return std::pair<std::string, std::string>(ip, port);
	}
} // namespace

namespace cgi
{
	const std::string CgiResponse::kCgiVersion = "1.1";

	// main constructor
	CgiResponse::CgiResponse(
		server::IRequest              &request,
		const conf::LocationConf      &location_conf,
		const struct sockaddr_storage *server,
		const struct sockaddr_storage *client
	)
		: AResponse(request),
		  location_conf_(location_conf),
		  body_writer_(request.GetBody()),
		  state_(kHeader),
		  pid_(-1)
	{
		log(COL_BOLD "=== Cgi Response Constructor ===" COL_END);
		if (location_conf_.GetCgiPath().empty() || server == NULL || client == NULL) {
			throw std::logic_error("cgi path empty");
		}
		ManagedFd child_fd;
		if (CreateUds(parent_fd_, child_fd).IsErr()) {
			throw http::InternalServerErrorException();
		}
		cgi_receiver_                 = server::Reciever(parent_fd_.GetFd());
		const std::string real_path   = TrimPathInfo(request_.Path());
		const std::string script_name = TryResolveIndexFilePath(
			location_conf_.GetRoot(), real_path, location_conf_.GetIndexFiles()
		);
		log("script_name: ", script_name);
		if (utils::EndWith(script_name, "/")) {
			throw http::ForbiddenException();
		}
		const std::string script_path = utils::JoinPath(location_conf_.GetRoot(), script_name);
		Stat              st          = TryStat(script_path);
		if (st.IsDirectory()) {
			ExecuteDirectoryRedirect(script_name + "/");
		} else if (st.IsRegularFile()) {
			Result<MetaEnvs> envs = SetMetaVariables(script_name, server, client);
			if (envs.IsErr()) {
				throw http::InternalServerErrorException();
			}
			if (ExecCgi(script_path, child_fd, envs.Val()).IsErr()) {
				throw http::InternalServerErrorException();
			}
		} else {
			throw http::NotFoundException();
		}
	}

	Result<void> CgiResponse::CreateUds(ManagedFd &parent_fd, ManagedFd &child_fd)
	{
		int fds[2];
		if (socketpair(AF_UNIX, SOCK_STREAM, 0, fds) == -1) {
			return Error();
		}
		parent_fd = ManagedFd(fds[0]);
		child_fd  = ManagedFd(fds[1]);
		// TODO NON_BLOCKING
		return Result<void>();
	}

	std::string CgiResponse::TrimPathInfo(const std::string &request_path)
	{
		typedef std::vector<ThinString> Strings;

		const std::string &root     = location_conf_.GetRoot();
		const Strings      segments = utils::TrimEmpty(Split(request_path, "/"));
		std::string        trimed   = "/";
		for (Strings::const_iterator it = segments.begin(); it != segments.end(); ++it) {
			trimed                        = utils::JoinPath(trimed, it->ToString());
			std::string          abs_path = utils::JoinPath(root, trimed);
			result::Result<Stat> st       = Stat::FromPath(abs_path);
			if (st.IsOk() && st.Val().IsRegularFile() && utils::IsReadablePath(abs_path)) {
				return trimed;
			}
		}
		return request_path;
	}

	response::PartialPath CgiResponse::TryResolveIndexFilePath(
		const response::FullPath             &root,
		const response::PartialPath          &request_path,
		const conf::LocationConf::IndexFiles &index_files
	)
	{
		result::Result<response::PartialPath> resolved =
			response::ResolveIndexFilePath(root, request_path, index_files);
		if (resolved.IsErr()) {
			result::ErrCode err = resolved.Err();
			if (err == Stat::kEAcces || err == Stat::kELoop) {
				throw http::ForbiddenException();
			} else if (err == Stat::kENotDir || err == Stat::kNoEnt || err == Stat::kENameTooLong) {
				throw http::NotFoundException();
			} else {
				throw http::InternalServerErrorException();
			}
		}
		return resolved.Val();
	}

	Result<MetaEnvs> CgiResponse::SetMetaVariables(
		const std::string             &script_name,
		const struct sockaddr_storage *server,
		const struct sockaddr_storage *client
	)
	{
		MetaEnvs                                     envs;
		Result<std::pair<std::string, std::string> > sv_ip_port = GetSocketAddress(server);
		Result<std::pair<std::string, std::string> > cl_ip_port = GetSocketAddress(client);

		if (sv_ip_port.IsErr() || cl_ip_port.IsErr()) {
			return Error();
		}
		SetContentLength(envs, request_);
		SetContentType(envs, request_);
		SetGatewayInterface(envs);
		SetPathEnvs(envs, request_, location_conf_.GetRoot(), script_name);
		SetQueryString(envs, request_);
		SetRemoteAddr(envs, cl_ip_port.Val().first);
		SetRequestMethod(envs, request_);
		SetServerName(envs, request_);
		SetServerPort(envs, sv_ip_port.Val().second);
		SetServerProtocol(envs);
		SetServerSoftware(envs);
		return envs;
	}

	Result<void>
	CgiResponse::ExecCgi(const std::string &script_path, ManagedFd &child_fd, const MetaEnvs &envs)
	{
		log("cgi execute", location_conf_.GetCgiPath().Value() + " " + script_path);
		pid_ = fork();
		switch (pid_) {
		case -1:
			return Error();
		case 0:
			ExecChild(script_path, child_fd, envs);
			exit(0); // not reach,
		default:
			child_fd.Close();
			return Result<void>();
		}
	}

	void CgiResponse::ExecuteDirectoryRedirect(const std::string &request_path)
	{
		log("no slash dir redirect", request_path);
		std::string page = utils::CreateDefaultPage(http::StatusCode::kMovedPermanently);
		MetaDataStorage::StoreStatusLine(http::kHttpVersion, http::StatusCode::kMovedPermanently);
		MetaDataStorage::StoreHeader("Server", http::kServerName);
		MetaDataStorage::StoreHeader("Content-Type", "text/html");
		MetaDataStorage::StoreHeader("Content-Length", utils::ToString(page.size()));
		MetaDataStorage::StoreHeader("Connection", request_.NeedToClose() ? "close" : "keep-alive");
		MetaDataStorage::StoreHeader("Location", CreateLocationUrl(request_path));
		MetaDataStorage::PushWithCrLf();
		push_back(page);
		is_finished_ = true;
	}

	std::string CgiResponse::CreateLocationUrl(const std::string &path) const
	{
		return "http://" + utils::JoinPath(request_.Authority(), path);
	}

	Stat CgiResponse::TryStat(const std::string &path) const
	{
		result::Result<Stat> st = Stat::FromPath(path);
		if (st.IsErr()) {
			result::ErrCode err = st.Err();
			if (err == Stat::kEAcces || err == Stat::kELoop) {
				throw http::ForbiddenException();
			} else if (err == Stat::kENotDir || err == Stat::kNoEnt || err == Stat::kENameTooLong) {
				throw http::NotFoundException();
			} else {
				throw http::InternalServerErrorException();
			}
		}
		return st.Val();
	}

	CgiResponse::~CgiResponse()
	{
		log(COL_BOLD COL_RED "CgiResponse destructor" COL_END);
		if (waitpid(pid_, NULL, WNOHANG) == 0) {
			kill(pid_, SIGKILL);
			waitpid(pid_, NULL, 0);
		}
	}

} // namespace cgi
