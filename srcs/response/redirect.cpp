#include "redirect.hpp"
#include "debug.hpp"
#include "http_define.hpp"
#include "webserv_utils.hpp"

namespace response
{
	Redirect::Redirect(const server::IRequest &request, const conf::LocationConf &location)
		: AResponse(request), location_(location)
	{
		log("Redirect", request.Path());
		std::pair<conf::StatusCode, conf::Url> redir       = location.GetRedirect().Value();
		std::string                            status_code = redir.first;
		std::string                            url         = redir.second;
		http::StatusCode::Code                 code =
			static_cast<http::StatusCode::Code>(utils::StrToLong(status_code).Val());
		std::string page = utils::CreateDefaultPage(code);
		MetaDataStorage::StoreStatusLine(http::kHttpVersion, code);
		MetaDataStorage::StoreHeader("Server", http::kServerName);
		MetaDataStorage::StoreHeader("Content-Type", "text/html");
		MetaDataStorage::StoreHeader("Content-Length", utils::ToString(page.size()));
		MetaDataStorage::StoreHeader("Connection", request_.NeedToClose() ? "close" : "keep-alive");
		MetaDataStorage::StoreHeader("Location", url);
		MetaDataStorage::PushWithCrLf();
		push_back(page);
		is_finished_ = true;
	}

	void Redirect::Perform(const event::Event &event)
	{
		throw std::logic_error("redirect perform");
		(void)event;
	}

	bool Redirect::HasFd() const
	{
		return false;
	}

	Emptiable<int> Redirect::GetFd() const
	{
		return Emptiable<int>();
	}

} // namespace response
