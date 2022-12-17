#include <cerrno>
#include <fcntl.h>

#include "debug.hpp"
#include "get_method.hpp"
#include "http_define.hpp"
#include "http_exceptions.hpp"
#include "webserv_utils.hpp"

namespace response
{
	GetMethod::GetMethod(const server::IRequest &request, const conf::LocationConf &location)
		: AResponse(request), location_(location), managed_fd_()
	{
		log("GET", request.Path());
		const std::string &root = location_.GetRoot();
		const std::string  path = utils::JoinPath(root, request_.Path());
		managed_fd_             = TryOpen(path);
		// TODO autoindx, index-files, redirect, headeres
		MetaDataStorage::StoreStatusLine(http::kHttpVersion, http::StatusCode::kOK);
		MetaDataStorage::StoreHeader("Server", http::kServerName);
		MetaDataStorage::StoreHeader("Connection", "keep-alive");
		MetaDataStorage::PushWithCrLf();
	}

	int GetMethod::TryOpen(const std::string &filename) const
	{
		int fd = open(filename.c_str(), O_RDONLY | O_NONBLOCK | O_CLOEXEC);
		if (fd == -1) {
			switch (errno) {
			case EACCES:
				throw http::ForbiddenException();
			case ENOENT:
			case ENAMETOOLONG:
				throw http::NotFoundException();
			default:
				throw http::InternalServerErrorException();
			}
		}
		return fd;
	}

	void GetMethod::Perform(const event::Event &event)
	{
		(void)event;
		ssize_t read_size = Read(managed_fd_.GetFd());
		if (read_size < 0) {
			throw http::InternalServerErrorException();
		}
		if (read_size == 0) {
			is_finished_ = true;
		}
	}

	bool GetMethod::HasFd() const
	{
		return true;
	}

	Emptiable<int> GetMethod::GetFd() const
	{
		return managed_fd_.GetFd();
	}

} // namespace response
