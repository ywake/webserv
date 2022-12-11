#include "cgi_response.hpp"
#include "http_exceptions.hpp"
#include "stat.hpp"
#include "webserv_utils.hpp"
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

extern char **environ;

namespace cgi
{
	bool IsEndWithSlash(const std::string &str);
	int  ErrCheck(int ret)
	{
		if (ret == -1) {
			throw http::InternalServerErrorException();
		}
		return ret;
	}

	// copy constructor
	CgiResponse::CgiResponse(const CgiResponse &other)
		: q_buffer::QueuingBuffer(other),
		  q_buffer::QueuingWriter(other),
		  q_buffer::QueuingReader(other),
		  request_(other.request_),
		  location_conf_(other.location_conf_),
		  resource_path_(other.resource_path_),
		  parent_fd_(other.parent_fd_),
		  child_fd_(other.child_fd_),
		  state_(other.state_)
	{}

	// main constructor
	CgiResponse::CgiResponse(server::IRequest &request, conf::LocationConf &location_conf)
		: q_buffer::QueuingWriter(),
		  q_buffer::QueuingReader(),
		  request_(request),
		  location_conf_(location_conf),
		  resource_path_(),
		  state_(kBeforeExec)
	{
		resource_path_ = GetResourcePath();
		int fds[2];
		if (socketpair(AF_UNIX, SOCK_STREAM, 0, fds)) {
			throw http::InternalServerErrorException();
		}
		parent_fd_ = ManagedFd(fds[0]);
		child_fd_  = ManagedFd(fds[1]);
		q_buffer::QueuingWriter::push_back(*request.GetBody());
	}

	CgiResponse::Path CgiResponse::GetResourcePath() const
	{
		Result<CgiResponse::Path> resource_path = FindResourcePath();
		if (resource_path.IsErr()) {
			throw http::NotFoundException();
		}
		result::Result<Stat> stat = Stat::FromPath(resource_path.Val());
		if (stat.IsErr()) {
			throw http::InternalServerErrorException();
		} else if (!stat.Val().IsRegularFile()) {
			throw http::ForbiddenException();
		}
		return resource_path.Val();
	}

	Result<CgiResponse::Path> CgiResponse::FindResourcePath() const
	{
		CgiResponse::Path base_path = utils::JoinPath(location_conf_.GetRoot(), request_.Path());
		if (IsEndWithSlash(base_path)) {
			std::vector<CgiResponse::Path> candidates = CombineIndexFiles(base_path);
			return FindAccessiblePathFromArray(candidates);
		} else {
			return GetAccessiblePath(base_path);
		}
	}

	bool IsEndWithSlash(const std::string &str)
	{
		return !str.empty() && utils::GetLastChar(str) == '/';
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
			return Error("Normalize path failed");
		}
		if (access(normalized_path.Val().c_str(), R_OK) < 0) {
			return Error("Access failed");
		}
		return normalized_path;
	}

	CgiResponse::~CgiResponse() {}

	CgiResponse &CgiResponse::operator=(const CgiResponse &other)
	{
		if (this == &other) {
			return *this;
		}
		request_       = other.request_;
		location_conf_ = other.location_conf_;
		resource_path_ = other.resource_path_;
		parent_fd_     = other.parent_fd_;
		child_fd_      = other.child_fd_;
		state_         = other.state_;
		q_buffer::QueuingWriter::operator=(other);
		q_buffer::QueuingReader::operator=(other);
		return *this;
	}

	/**
	 * IResponse
	 */
	void CgiResponse::Perform(const event::Event &event)
	{
		switch (state_) {
		case kBeforeExec:
			if (event.event_type & event::Event::kWrite) {
				OnWriteCgiInput();
			}
			break;
		case kAfterExec:
			if (event.event_type & event::Event::kRead) {
				OnReadCgiOutput();
			}
			break;
		default:
			break;
		}
	}

	void CgiResponse::OnWriteCgiInput()
	{
		Result<void> res = Write(parent_fd_.GetFd());
		if (res.IsErr()) {
			throw http::InternalServerErrorException();
		}
		if (q_buffer::QueuingWriter::empty()) {
			ExecCgi();
			state_ = kAfterExec;
		}
	}

	void CgiResponse::ExecCgi()
	{
		pid_t pid = ErrCheck(fork());
		switch (pid) {
		case 0:
			ExecCgiChild();
			break;
		default:
			ExecCgiParent(pid);
			break;
		}
	}

	void CgiResponse::ExecCgiChild()
	{
		std::vector<char *> args;
		std::vector<char *> envs;
		CreateArgs(args);
		CreateEnvs(envs);

		ErrCheck(dup2(parent_fd_.GetFd(), STDIN_FILENO));
		ErrCheck(dup2(child_fd_.GetFd(), STDOUT_FILENO));
		ErrCheck(close(parent_fd_.GetFd()));
		ErrCheck(close(child_fd_.GetFd()));
		ErrCheck(execve(resource_path_.c_str(), args.data(), envs.data()));
	}

	void CgiResponse::CreateArgs(std::vector<char *> args)
	{
		Emptiable<std::string> cgi_path = location_conf_.GetCgiPath();
		if (cgi_path.empty()) {
			// cgi_pathがemptyでない時コンストラクトされるので、ありえないはず
			throw http::InternalServerErrorException();
		}
		args.push_back(const_cast<char *>(cgi_path.Value().c_str()));
		args.push_back(const_cast<char *>(resource_path_.c_str()));
		args.push_back(NULL);
	}

	void CgiResponse::CreateEnvs(std::vector<char *> envs)
	{
		for (size_t i = 0; environ[i] != NULL; ++i) {
			envs.push_back(environ[i]);
		}
		envs.push_back(NULL);
	}

	void CgiResponse::ExecCgiParent(pid_t pid)
	{
		(void)pid;
		ErrCheck(close(parent_fd_.GetFd()));
	}

	void CgiResponse::OnReadCgiOutput() {}

	Result<void> CgiResponse::Send(int fd)
	{
		return Write(fd);
	}

	bool CgiResponse::HasReadyData() const
	{
		return !QueuingBuffer::empty();
	}

	bool CgiResponse::HasFd() const
	{
		return parent_fd_.GetFd() != ManagedFd::kNofd;
	}

	Emptiable<int> CgiResponse::GetFd() const
	{
		return parent_fd_.GetFd();
	}

	std::size_t CgiResponse::size() const
	{
		return q_buffer::QueuingBuffer::size();
	}

	bool CgiResponse::IsFinished() const
	{
		return state_ == kFinished;
	}

} // namespace cgi
