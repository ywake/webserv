#include <cerrno>
#include <unistd.h>

#include "debug.hpp"
#include "delete_method.hpp"
#include "http_exceptions.hpp"
#include "stat.hpp"

namespace
{
	Stat TryStat(const std::string &path)
	{
		result::Result<Stat> st = Stat::FromPath(path);
		if (st.IsErr()) {
			result::ErrCode err = st.Err();
			if (err == Stat::kEAcces || err == Stat::kELoop) {
				throw http::ForbiddenException();
			} else if (err == Stat::kENotDir || err == Stat::kNoEnt || err == Stat::kENameTooLong) {
				throw http::NotFoundException();
			} else {
				throw http::InternalServerErrorException();
			}
		}
		return st.Val();
	}
} // namespace

namespace response
{
	DeleteMethod::DeleteMethod(const server::IRequest &request, const conf::LocationConf &location)
		: AResponse(request), location_(location)
	{
		log("DELETE", request.Path());
		const std::string &root = location_.GetRoot();
		const std::string  path = utils::JoinPath(root, request_.Path());
		Stat               st   = TryStat(path);
		if (st.IsDirectory()) {
			throw http::ForbiddenException();
		} else if (!st.IsRegularFile()) {
			throw http::NotFoundException();
		} else if (unlink(path.c_str()) == -1) {
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

	AResponse::FinEventType DeleteMethod::Perform(const event::Event &event)
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
