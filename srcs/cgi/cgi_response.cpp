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

	static bool IsEndWithSlash(const std::string &str);

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
		  resource_path_(),
		  state_(kHeader),
		  pid_(-1)
	{
		log(COL_BOLD "=== Cgi Response Constructor ===" COL_END);
		resource_path_ = GetResourcePath();
		log("resource_path: ", resource_path_);
		int fds[2];
		if (socketpair(AF_UNIX, SOCK_STREAM, 0, fds) == -1) {
			throw http::InternalServerErrorException();
		}
		parent_fd_   = ManagedFd(fds[0]);
		child_fd_    = ManagedFd(fds[1]);
		body_writer_ = server::BodyWriter(request.GetBody());
		ExecCgi();
		cgi_receiver_ = server::Reciever(parent_fd_.GetFd());
	}

	CgiResponse::Path CgiResponse::GetResourcePath() const
	{
		Result<CgiResponse::Path> resource_path = FindResourcePath();
		if (resource_path.IsErr()) {
			log(COL_RED "resource_path is err" COL_END);
			throw http::NotFoundException();
		}
		result::Result<Stat> stat = Stat::FromPath(resource_path.Val());
		if (stat.IsErr()) {
			log(COL_RED "stat error" COL_END);
			throw http::InternalServerErrorException();
		} else if (!stat.Val().IsRegularFile()) {
			log(COL_RED "Is not a regular file." COL_END);
			throw http::ForbiddenException();
		}
		return resource_path.Val();
	}

	Result<CgiResponse::Path> CgiResponse::FindResourcePath() const
	{
		CgiResponse::Path base_path = utils::JoinPath(location_conf_.GetRoot(), request_.Path());
		log("base_path: ", base_path);
		if (IsEndWithSlash(base_path)) {
			std::vector<CgiResponse::Path> candidates = CombineIndexFiles(base_path);
			return FindAccessiblePathFromArray(candidates);
		} else {
			return GetAccessiblePath(base_path);
		}
	}

	std::vector<CgiResponse::Path> CgiResponse::CombineIndexFiles(const CgiResponse::Path &base_path
	) const
	{
		std::vector<CgiResponse::Path> path_array;
		std::vector<CgiResponse::Path> index_files = location_conf_.GetIndexFiles();
		for (size_t i = 0; i < index_files.size(); ++i) {
			CgiResponse::Path combined_path = utils::JoinPath(base_path, index_files[i]);
			path_array.push_back(combined_path);
		}
		return path_array;
	}

	Result<CgiResponse::Path>
	CgiResponse::FindAccessiblePathFromArray(const std::vector<Path> &candidates) const
	{
		for (size_t i = 0; i < candidates.size(); ++i) {
			Result<CgiResponse::Path> accessible_path = GetAccessiblePath(candidates[i]);
			if (accessible_path.IsOk()) {
				return accessible_path;
			}
		}
		return Error("No accessible path");
	}

	Result<CgiResponse::Path> CgiResponse::GetAccessiblePath(const CgiResponse::Path &path) const
	{
		Result<CgiResponse::Path> normalized_path = utils::NormalizePath(path);
		if (normalized_path.IsErr()) {
			log(COL_RED "Normalize path failed" COL_END);
			return Error("Normalize path failed");
		}
		if (access(normalized_path.Val().c_str(), R_OK) < 0) {
			log(COL_RED "access failed" COL_END);
			return Error("Access failed");
		}
		return normalized_path;
	}

	void CgiResponse::ExecCgi()
	{
		log("cgi execute", location_conf_.GetCgiPath().Value() + " " + resource_path_);
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

	static bool IsEndWithSlash(const std::string &str)
	{
		return !str.empty() && utils::GetLastChar(str) == '/';
	}
} // namespace cgi
