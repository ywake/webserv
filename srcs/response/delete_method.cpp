#include <cerrno>
#include <unistd.h>

#include "debug.hpp"
#include "delete_method.hpp"
#include "http_exceptions.hpp"

namespace response
{
	DeleteMethod::DeleteMethod(const server::IRequest &request, const conf::LocationConf &location)
		: AResponse(request), location_(location)
	{
		log("DELETE", request.Path());
		const std::string &root = location_.GetRoot();
		const std::string  path = utils::JoinPath(root, request_.Path());
		if (unlink(path.c_str()) == -1) {
			switch (errno) {
			case EACCES:
			case EPERM:
			case EISDIR:
				throw http::ForbiddenException();
			case ENAMETOOLONG:
			case ENOENT:
			case ENOTDIR:
				throw http::NotFoundException();
			default:
				throw http::InternalServerErrorException();
			}
		}
		// TODO headers
		MetaDataStorage::StoreStatusLine(http::kHttpVersion, http::StatusCode::kOK);
		MetaDataStorage::StoreHeader("Server", http::kServerName);
		MetaDataStorage::StoreHeader("Connection", NeedToClose() ? "close" : "keep-alive");
		MetaDataStorage::PushWithCrLf();
		is_finished_ = true;
	}

	void DeleteMethod::Perform(const event::Event &event)
	{
		throw std::logic_error("delete perform");
		(void)event;
	}

	bool DeleteMethod::HasFd() const
	{
		return false;
	}

	Emptiable<int> DeleteMethod::GetFd() const
	{
		return Emptiable<int>();
	}

} // namespace response
