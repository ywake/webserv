#include <cerrno>
#include <fcntl.h>

#include "debug.hpp"
#include "error_response.hpp"
#include "http_define.hpp"
#include "http_exceptions.hpp"
#include "mime_type.hpp"
#include "stat.hpp"

namespace response
{
	ErrorResponse::ErrorResponse(
		const server::IRequest &request,
		const http::StatusCode &status_code,
		const conf::ServerConf &conf
	)
		: AResponse(request), config_(conf)
	{
		log("error res construct", status_code.GetCode());
		need_to_close_ = status_code == http::StatusCode::kBadRequest;
		MetaDataStorage::StoreStatusLine(http::kHttpVersion, status_code);
		MetaDataStorage::StoreHeader("Server", http::kServerName);
		MetaDataStorage::StoreHeader("Connection", NeedToClose() ? "close" : "keep-alive");
		if (status_code == http::StatusCode::kMethodNotAllowed ||
			status_code == http::StatusCode::kNotImplemented) {
			const conf::LocationConf &location = config_.FindMatchingLocationConf(request.Path());
			const conf::LocationConf::AllowMethods &methods = location.GetAllowMethods();
			MetaDataStorage::StoreHeader("Allow", methods.begin(), methods.end());
		}
		if (OpenErrorPage(status_code).IsErr()) {
			std::string page = utils::CreateDefaultPage(status_code);
			MetaDataStorage::StoreHeader("Content-Type", "text/html");
			MetaDataStorage::StoreHeader("Content-Length", utils::ToString(page.size()));
			MetaDataStorage::PushWithCrLf();
			push_back(page);
			is_finished_ = true;
		}
	}

	Result<void> ErrorResponse::OpenErrorPage(const http::StatusCode &code)
	{
		const conf::ServerConf::ErrorPages::const_iterator &it =
			config_.GetErrorPages().find(code.GetCodeStr());
		if (it == config_.GetErrorPages().end()) {
			return Error();
		}
		const char          *path = it->second.c_str();
		result::Result<Stat> st   = Stat::FromPath(path);
		if (st.IsErr() || !st.Val().IsRegularFile()) {
			return Error();
		}
		int fd = open(path, O_RDONLY | O_NONBLOCK | O_CLOEXEC);
		if (fd == -1) {
			return Error();
		}
		managed_fd_ = fd;
		MetaDataStorage::StoreHeader("Content-Type", http::MimeType::GetMimeType(path));
		MetaDataStorage::StoreHeader("Content-Length", utils::ToString(st.Val().FileSize()));
		MetaDataStorage::PushWithCrLf();
		return Result<void>();
	}

	AResponse::FinEventType ErrorResponse::Perform(const event::Event &event)
	{
		FinEventType fin = event.event_type & event::Event::kWrite;

		if (!(event.event_type & event::Event::kRead)) {
			return fin;
		}
		ssize_t read_size = Read(managed_fd_.GetFd());
		if (read_size < 0) {
			throw http::InternalServerErrorException();
		}
		if (read_size == 0) {
			fin |= event::Event::kRead;
			is_finished_ = true;
		}
		return fin;
	}

	bool ErrorResponse::HasFd() const
	{
		return managed_fd_.GetFd() != ManagedFd::kNofd;
	}

	Emptiable<int> ErrorResponse::GetFd() const
	{
		return managed_fd_.GetFd();
	}

} // namespace response
