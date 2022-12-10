#include "cgi_response.hpp"
#include "http_exceptions.hpp"
#include "stat.hpp"
#include "webserv_utils.hpp"
#include <sys/socket.h>
#include <unistd.h>

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
		: request_(other.request_),
		  location_conf_(other.location_conf_),
		  resource_path_(other.resource_path_)
	{}

	// main constructor
	CgiResponse::CgiResponse(server::IRequest &request, conf::LocationConf &location_conf)
		: request_(request), location_conf_(location_conf), resource_path_(), state_(kBeforeExec)
	{
		GetResourcePath();
		int fds[2];
		ErrCheck(socketpair(AF_UNIX, SOCK_STREAM, 0, fds));
		cgi_fd_in_  = ManagedFd(fds[0]);
		cgi_fd_out_ = ManagedFd(fds[1]);
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
		return *this;
	}

	/**
	 * IResponse
	 */
	void CgiResponse::Perform(const event::Event &event)
	{
		switch (state_) {
		case CgiResponse::kBeforeExec:
			if (event & event::kWrite) {
				OnWriteCgiInput();
			}
			break;
		case CgiResponse::kAfterExec:
			if (event & event::kRead) {
				OnReadCgiOutput();
			}
			break;
		}
	}

	void CgiResponse::OnWriteCgiInput() {}
	void CgiResponse::OnReadCgiOutput()
	{
		ssize_t read_size = Read(cgi_fd_out_.GetFd());
		switch (read_size) {
		case -1:
			throw http::InternalServerErrorException();
			break;
		case 0:
			state_ = kFinished;
			break;
		default:
			break;
		}
	}

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
		}
	}

	Emptiable<int> CgiResponse::GetFd() const
	{
		switch (state_) {
		case CgiResponse::kBeforeExec:
			return cgi_fd_in_.GetFd();
		case CgiResponse::kAfterExec:
			return cgi_fd_out_.GetFd();
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
