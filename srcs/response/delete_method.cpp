#include <cerrno>
#include <unistd.h>

#include "delete_method.hpp"
#include "http_exceptions.hpp"

namespace response
{
	DeleteMethod::DeleteMethod(const server::IRequest &request, const conf::LocationConf &location)
		: AResponse(request), location_(location)
	{
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
	}

	void DeleteMethod::Perform(const event::Event &event)
	{
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
