#include "cgi_response.hpp"
#include "http_exceptions.hpp"
#include "stat.hpp"
#include "webserv_utils.hpp"
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

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
		resource_path_ = GetResourcePath();
		int fds[2];
		if (socketpair(AF_UNIX, SOCK_STREAM, 0, fds)) {
			throw http::InternalServerErrorException();
		}
		parent_fd_ = ManagedFd(fds[0]);
		child_fd_  = ManagedFd(fds[1]);
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

	void CgiResponse::OnWriteCgiInput() {}
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
		parent_fd_ != ManagedFd::kNofd;
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
