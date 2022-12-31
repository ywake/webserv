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

	// copy constructor
	// CgiResponse::CgiResponse(const CgiResponse &other)
	// 	: AResponse(other),
	// 	  location_conf_(other.location_conf_),
	// 	  resource_path_(other.resource_path_),
	// 	  parent_fd_(other.parent_fd_),
	// 	  child_fd_(other.child_fd_),
	// 	  body_writer_(other.body_writer_),
	// 	  cgi_receiver_(),
	// 	  field_parser_()
	// {}

	// main constructor
	CgiResponse::CgiResponse(server::IRequest &request, const conf::LocationConf &location_conf)
		: AResponse(request),
		  location_conf_(location_conf),
		  script_name_(),
		  state_(kHeader),
		  pid_(-1)
	{
		log(COL_BOLD "=== Cgi Response Constructor ===" COL_END);
		if (location_conf_.GetCgiPath().empty()) {
			throw std::logic_error("cgi path empty");
		}
		if (CreateUds(parent_fd_, child_fd_).IsErr()) {
			throw http::InternalServerErrorException();
		}
		body_writer_                = server::BodyWriter(request.GetBody());
		cgi_receiver_               = server::Reciever(parent_fd_.GetFd());
		const std::string real_path = TrimPathInfo(request_.Path());
		script_name_                = TryResolveIndexFilePath(
            location_conf_.GetRoot(), real_path, location_conf_.GetIndexFiles()
        );
		log("script_name: ", script_name_);
		ExecCgi();
	}

	Result<void> CgiResponse::CreateUds(ManagedFd &parent_fd, ManagedFd &child_fd)
	{
		int fds[2];
		if (socketpair(AF_UNIX, SOCK_STREAM, 0, fds) == -1) {
			return Error();
		}
		parent_fd = ManagedFd(fds[0]);
		child_fd  = ManagedFd(fds[1]);
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

	void CgiResponse::ExecCgi()
	{
		log("cgi execute",
			location_conf_.GetCgiPath().Value() + " " + location_conf_.GetRoot() + script_name_);
		pid_ = fork();
		switch (pid_) {
		case -1:
			throw http::InternalServerErrorException();
			break;
		case 0:
			log("child process");
			ChildProcess();
			break;
		default:
			ParentProcess(pid_);
			break;
		}
	}

	void CgiResponse::ParentProcess(pid_t pid)
	{
		(void)pid;
		child_fd_.Close();
	}

	CgiResponse::~CgiResponse()
	{
		log(COL_BOLD COL_RED "CgiResponse destructor" COL_END);
		if (waitpid(pid_, NULL, WNOHANG) == 0) {
			kill(pid_, SIGKILL);
			waitpid(pid_, NULL, 0);
		}
		// TODO Kill
	}

	// CgiResponse &CgiResponse::operator=(const CgiResponse &other)
	// {
	// 	if (this == &other) {
	// 		return *this;
	// 	}
	// 	request_       = other.request_;
	// 	location_conf_ = other.location_conf_;
	// 	resource_path_ = other.resource_path_;
	// 	parent_fd_     = other.parent_fd_;
	// 	child_fd_      = other.child_fd_;
	// 	is_finished_   = other.is_finished_;
	// 	body_writer_   = other.body_writer_;
	// 	q_buffer::QueuingWriter::operator=(other);
	// 	q_buffer::QueuingReader::operator=(other);
	// 	return *this;
	// }
} // namespace cgi
