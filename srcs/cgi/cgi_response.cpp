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
		  cgi_fd_in_(other.cgi_fd_in_),
		  cgi_fd_out_(other.cgi_fd_out_),
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
		GetResourcePath();
		int fds[2];
		ErrCheck(socketpair(AF_UNIX, SOCK_STREAM, 0, fds));
		cgi_fd_in_  = ManagedFd(fds[0]);
		cgi_fd_out_ = ManagedFd(fds[1]);
		q_buffer::QueuingWriter::push_back(*request.GetBody());
	}

	void CgiResponse::GetResourcePath()
	{
		std::vector<CgiResponse::Path> candidate_paths;
		GetResourcePathCandidates(candidate_paths);
		Result<CgiResponse::Path> accessible_path = FindAccessible(candidate_paths);
		if (accessible_path.IsErr()) {
			throw http::NotFoundException();
		}
		result::Result<Stat> stat = Stat::FromPath(accessible_path.Val());
		if (stat.IsErr()) {
			CgiStatErrorHandler(stat.Err());
		}
		CgiStatFileTypeHandler(stat.Val());
		resource_path_ = accessible_path.Val();
	}

	void CgiResponse::GetResourcePathCandidates(std::vector<CgiResponse::Path> &candidates) const
	{
		CgiResponse::Path base_path = location_conf_.GetRoot() + request_.Path();
		if (IsEndWithSlash(base_path)) {
			candidates = CombineIndexFiles(base_path);
		} else {
			candidates.push_back(base_path);
		}
	}

	bool IsEndWithSlash(const std::string &str)
	{
		return !str.empty() && *str.end() == '/';
	}

	std::vector<CgiResponse::Path> CgiResponse::CombineIndexFiles(const CgiResponse::Path &base_path
	) const
	{
		std::vector<CgiResponse::Path> path_array;
		std::vector<CgiResponse::Path> index_files = location_conf_.GetIndexFiles();
		for (size_t i = 0; i < index_files.size(); ++i) {
			CgiResponse::Path combined_path = base_path + index_files[i];
			path_array.push_back(combined_path);
		}
		return path_array;
	}

	Result<CgiResponse::Path>
	CgiResponse::FindAccessible(const std::vector<CgiResponse::Path> &candidates) const
	{
		for (size_t i = 0; i < candidates.size(); ++i) {
			Result<CgiResponse::Path> path = utils::NormalizePath(candidates[i]);
			if (path.IsErr()) {
				continue;
			}
			if (access(path.Val().c_str(), R_OK) == 0) {
				return candidates[i];
			}
		}
		return Error("No accessible resource found");
	}

	void CgiResponse::CgiStatErrorHandler(const result::ErrCode &error_code) const
	{
		if (error_code == Stat::kEAcces) {
			throw http::ForbiddenException();
		} else if (error_code == Stat::kNoEnt) {
			throw http::NotFoundException();
		} else {
			throw http::InternalServerErrorException();
		}
	}

	void CgiResponse::CgiStatFileTypeHandler(const Stat &stat) const
	{
		switch (stat.GetFileType()) {
		case Stat::kRegularFile:
			break;
		case Stat::kDirectory:
			throw http::ForbiddenException();
		default:
			throw http::NotFoundException();
		}
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
		cgi_fd_in_     = other.cgi_fd_in_;
		cgi_fd_out_    = other.cgi_fd_out_;
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
		Result<void> res = Write(cgi_fd_in_.GetFd());
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

		ErrCheck(dup2(cgi_fd_in_.GetFd(), STDIN_FILENO));
		ErrCheck(dup2(cgi_fd_out_.GetFd(), STDOUT_FILENO));
		ErrCheck(close(cgi_fd_in_.GetFd()));
		ErrCheck(close(cgi_fd_out_.GetFd()));
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
		ErrCheck(close(cgi_fd_in_.GetFd()));
	}

	void CgiResponse::OnReadCgiOutput() {}

	Result<void> CgiResponse::Send(int fd)
	{
		(void)fd;
		return Result<void>();
	}

	bool CgiResponse::HasReadyData() const
	{
		return false;
	}

	bool CgiResponse::HasFd() const
	{
		switch (state_) {
		case CgiResponse::kBeforeExec:
			return cgi_fd_in_.GetFd() != ManagedFd::kNofd;
		case CgiResponse::kAfterExec:
			return cgi_fd_out_.GetFd() != ManagedFd::kNofd;
		default:
			return false;
		}
	}

	Emptiable<int> CgiResponse::GetFd() const
	{
		switch (state_) {
		case CgiResponse::kBeforeExec:
			return cgi_fd_in_.GetFd();
		case CgiResponse::kAfterExec:
			return cgi_fd_out_.GetFd();
		default:
			return Emptiable<int>();
		}
	}

	std::size_t CgiResponse::size() const
	{
		return 0;
	}

	bool CgiResponse::IsFinished() const
	{
		return state_ == kFinished;
	}

} // namespace cgi
