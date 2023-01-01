#include "cgi_response.hpp"
#include "debug.hpp"
#include "http_exceptions.hpp"
#include "stat.hpp"
#include "webserv_utils.hpp"
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

#include <vector>

namespace cgi
{
	const std::string CgiResponse::kCgiVersion = "1.1";

	// main constructor
	CgiResponse::CgiResponse(server::IRequest &request, const conf::LocationConf &location_conf)
		: AResponse(request), location_conf_(location_conf), state_(kHeader), pid_(-1)
	{
		log(COL_BOLD "=== Cgi Response Constructor ===" COL_END);
		if (location_conf_.GetCgiPath().empty()) {
			throw std::logic_error("cgi path empty");
		}
		ManagedFd child_fd;
		if (CreateUds(parent_fd_, child_fd).IsErr()) {
			throw http::InternalServerErrorException();
		}
		body_writer_                  = server::BodyWriter(request.GetBody());
		cgi_receiver_                 = server::Reciever(parent_fd_.GetFd());
		const std::string real_path   = TrimPathInfo(request_.Path());
		const std::string script_name = TryResolveIndexFilePath(
			location_conf_.GetRoot(), real_path, location_conf_.GetIndexFiles()
		);
		log("script_name: ", script_name);
		if (ExecCgi(script_name, child_fd).IsErr()) {
			throw http::InternalServerErrorException();
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

	Result<void> CgiResponse::ExecCgi(const std::string &script_name, ManagedFd &child_fd)
	{
		log("cgi execute",
			location_conf_.GetCgiPath().Value() + " " + location_conf_.GetRoot() + script_name);
		pid_ = fork();
		switch (pid_) {
		case -1:
			return Error();
		case 0:
			ExecChild(script_name, child_fd);
			exit(0); // not reach,
		default:
			child_fd.Close();
			return Result<void>();
		}
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
