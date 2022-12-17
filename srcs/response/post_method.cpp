#include <cerrno>
#include <cstdio>
#include <cstring>
#include <fcntl.h>

#include "debug.hpp"
#include "http_exceptions.hpp"
#include "post_method.hpp"

namespace response
{
	PostMethod::PostMethod(const server::IRequest &request, const conf::LocationConf &location)
		: AResponse(request),
		  location_(location),
		  managed_fd_(),
		  filename_(),
		  body_writer_(request.GetBody())
	{
		log("POST", request.Path());
		if (utils::EndWith(request_.Path(), "/")) {
			throw http::ForbiddenException();
		}
		Result<std::string> time_stamp = utils::CreateTimeStamp();
		if (time_stamp.IsErr()) {
			throw http::InternalServerErrorException();
		}
		const std::string  uniq_path = request_.Path() + "_" + time_stamp.Val();
		const std::string &root      = location_.GetRoot();
		filename_                    = utils::JoinPath(root, uniq_path);
		managed_fd_                  = TryOpen(filename_);
		MetaDataStorage::StoreStatusLine(http::kHttpVersion, http::StatusCode::kCreated);
		MetaDataStorage::StoreHeader("Location", uniq_path); // TODO create uri
		MetaDataStorage::StoreHeader("Server", http::kServerName);
		MetaDataStorage::StoreHeader("Connection", "keep-alive");
		// TODO other headers 何必要か分からん
	}

	int PostMethod::TryOpen(const std::string &filename) const
	{
		int open_flags = O_CREAT | O_WRONLY | O_EXCL | O_NONBLOCK | O_CLOEXEC;
		int fd         = open(filename.c_str(), open_flags, S_IRUSR | S_IWUSR);
		if (fd == -1) {
			switch (errno) {
			case EACCES:
			case EEXIST:
			case ENXIO:
			case ENOTDIR:
			case ENOENT:
			case ENAMETOOLONG:
				throw http::ForbiddenException();
			default:
				throw http::InternalServerErrorException();
			}
		}
		return fd;
	}

	void PostMethod::Perform(const event::Event &event)
	{
		(void)event;
		if (body_writer_.IsFinished()) {
			is_finished_ = true;
			MetaDataStorage::PushWithCrLf();
			return;
		}
		Result<void> res = body_writer_.Write(managed_fd_.GetFd());
		if (res.IsErr()) {
			is_finished_ = true;
			remove(filename_.c_str());
			throw http::InternalServerErrorException();
		}
	}

	bool PostMethod::HasFd() const
	{
		return true;
	}

	Emptiable<int> PostMethod::GetFd() const
	{
		return managed_fd_.GetFd();
	}

} // namespace response
