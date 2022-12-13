#include "cgi_response.hpp"
#include "get_resource_pair.hpp"
#include "http_exceptions.hpp"
#include "stat.hpp"
#include "webserv_utils.hpp"
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

namespace cgi
{
	const std::string CgiResponse::kCgiVersion = "1.1";

	static void                ValidateResourcePair(const Result<CgiResponse::PathPair> &path_pair);
	static CgiResponse::FdPair GetSocketFdPair();

	// copy constructor
	CgiResponse::CgiResponse(const CgiResponse &other)
		: q_buffer::QueuingBuffer(other),
		  q_buffer::QueuingWriter(other),
		  q_buffer::QueuingReader(other),
		  request_(other.request_),
		  location_conf_(other.location_conf_),
		  script_path_(other.script_path_),
		  path_info_(other.path_info_),
		  parent_fd_(other.parent_fd_),
		  child_fd_(other.child_fd_),
		  is_finished_(false)
	{}

	// main constructor
	CgiResponse::CgiResponse(server::IRequest &request, conf::LocationConf &location_conf)
		: q_buffer::QueuingWriter(),
		  q_buffer::QueuingReader(),
		  request_(request),
		  location_conf_(location_conf),
		  is_finished_(false)
	{
		Result<CgiResponse::PathPair> path_pair = GetResourcePair(location_conf_, request_);
		ValidateResourcePair(path_pair);
		script_path_                 = path_pair.Val().first;
		path_info_                   = path_pair.Val().second;
		CgiResponse::FdPair sockpair = GetSocketFdPair();
		parent_fd_                   = ManagedFd(sockpair.first);
		child_fd_                    = ManagedFd(sockpair.second);
		q_buffer::QueuingWriter::push_back(*request.GetBody());
		ExecCgi();
	}

	CgiResponse::~CgiResponse() {}

	CgiResponse &CgiResponse::operator=(const CgiResponse &other)
	{
		if (this == &other) {
			return *this;
		}
		request_       = other.request_;
		location_conf_ = other.location_conf_;
		script_path_   = other.script_path_;
		path_info_     = other.path_info_;
		parent_fd_     = other.parent_fd_;
		child_fd_      = other.child_fd_;
		is_finished_   = other.is_finished_;
		q_buffer::QueuingWriter::operator=(other);
		q_buffer::QueuingReader::operator=(other);
		return *this;
	}

	void ValidateResourcePair(const Result<CgiResponse::PathPair> &path_pair)
	{
		if (path_pair.IsErr()) {
			throw http::NotFoundException();
		}
		CgiResponse::Path    script_path = path_pair.Val().first;
		result::Result<Stat> stat        = Stat::FromPath(script_path);
		if (stat.IsErr()) {
			throw http::InternalServerErrorException();
		} else if (!stat.Val().IsRegularFile()) {
			throw http::ForbiddenException();
		}
	}

	static CgiResponse::FdPair GetSocketFdPair()
	{
		int fds[2];
		if (socketpair(AF_UNIX, SOCK_STREAM, 0, fds)) {
			throw http::InternalServerErrorException();
		}
		return CgiResponse::FdPair(fds[0], fds[1]);
	}
} // namespace cgi
