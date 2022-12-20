#include <cerrno>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "autoindex.hpp"
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
		if (utils::EndWith(path, "/")) {
			ExecuteAutoIndex(root, request_.Path());
		} else {
			PrepareSendFile(path);
		}
		// TODO autoindx, index-files, redirect, headeres
	}

	void GetMethod::PrepareSendFile(const std::string &path)
	{
		managed_fd_ = TryOpen(path);
		MetaDataStorage::StoreStatusLine(http::kHttpVersion, http::StatusCode::kOK);
		MetaDataStorage::StoreHeader("Server", http::kServerName);
		MetaDataStorage::StoreHeader("Connection", "keep-alive");
		MetaDataStorage::PushWithCrLf();
	}

	void GetMethod::ExecuteAutoIndex(const std::string &root, const std::string &request_path)
	{
		std::string autoindex = TryCreateAutoIndex(root, request_path);
		MetaDataStorage::StoreStatusLine(http::kHttpVersion, http::StatusCode::kOK);
		MetaDataStorage::StoreHeader("Server", http::kServerName);
		MetaDataStorage::StoreHeader("Connection", "keep-alive");
		MetaDataStorage::StoreHeader("Content-Lengh", utils::ToString(autoindex.size()));
		MetaDataStorage::PushWithCrLf();
		push_back(autoindex);
	}

	std::string
	GetMethod::TryCreateAutoIndex(const std::string &root, const std::string &request_path)
	{
		result::Result<std::string> autoindex = autoindex::CreateAutoIndex(root, request_path);
		if (autoindex.IsErr()) {
			log("autoindx", autoindex.Err());
			if (autoindex.Err() == err::opendir::kEAccess) {
				throw http::ForbiddenException();
			} else if (autoindex.Err() == err::opendir::kENoEnt || autoindex.Err() == err::opendir::kENotDir) {
				throw http::NotFoundException();
			} else {
				throw http::InternalServerErrorException();
			}
		}
		return autoindex.Val();
	}

	int GetMethod::TryOpen(const std::string &filename) const
	{
		int fd = open(filename.c_str(), O_RDONLY | O_NONBLOCK | O_CLOEXEC);
		if (fd == -1) {
			switch (errno) {
			case EACCES:
			case ENXIO:
				throw http::ForbiddenException();
			case ENOTDIR:
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
		return managed_fd_.GetFd() != ManagedFd::kNofd;
	}

	Emptiable<int> GetMethod::GetFd() const
	{
		return managed_fd_.GetFd();
	}

} // namespace response
