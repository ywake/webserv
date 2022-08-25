#ifndef REQUEST_TARGET_HPP
#define REQUEST_TARGET_HPP
#include "result.hpp"
#include "uri.hpp"
#include "uri_abnf.hpp"
#include <cstdlib>
#include <vector>

class RequestTarget // TODO: Abstruct説
{
  public:
	enum RequestForm { ORIGIN, ABSOLUTE, AUTHORITY, ASTERISK };
	RequestForm form_type_;
	URI request_target_; // TODO: variable name

	RequestTarget() : form_type_(), request_target_() {}

	//[FIX]
	//例外投げてるけど、Errorの有無を変数で持ってコンストラクタ呼び出し後にチェックの方が良い？
	RequestTarget(std::string uri)
	{
		if (uri.empty()) {
			throw Error("400");
		}
		SpecifyForm(uri);
		switch (form_type_) {
		case ORIGIN:
			ParseOriginForm(uri);
			break;
		case ABSOLUTE:
			break;
		case AUTHORITY:
			break;
		case ASTERISK:
			break;
		default:
			break;
		}
	}

	RequestTarget(RequestForm form_type, URI request_target)
		: form_type_(form_type), request_target_(request_target)
	{
	}

	bool operator==(const RequestTarget &rhs) const
	{
		return form_type_ == rhs.form_type_ && request_target_ == rhs.request_target_;
	}

	bool operator!=(const RequestTarget &rhs) const
	{
		return !(*this == rhs);
	}

	void SpecifyForm(const std::string &uri)
	{
		if (uri == "*") {
			form_type_ = ASTERISK;
		} else if (uri.at(0) == '/') {
			form_type_ = ORIGIN;
		} else if (uri.find("://") != std::string::npos) {
			form_type_ = ABSOLUTE;
		} else if (uri.find(":") != std::string::npos) {
			form_type_ = AUTHORITY;
		} else {
			throw Error("400");
		}
	}

	typedef std::pair<std::string, std::string> StrPair;
	StrPair DivideStr(const std::string &str, const std::string &delim)
	{
		std::size_t boundary = str.find(delim);
		bool has_no_second = delim.empty() || boundary == std::string::npos;
		if (has_no_second) {
			return StrPair(str, "");
		}
		std::string left = str.substr(0, boundary);
		std::string right = str.substr(boundary + delim.size());
		return StrPair(left, right);
	}

	// origin-form
	// = absolute-path [ "?" query ]
	void ParseOriginForm(std::string uri)
	{
		StrPair path_query = DivideStr(uri, "?");

		if (!ABNF::IsPathAbsolute(path_query.first) || !ABNF::IsQuery(path_query.second)) {
			throw Error("400");
		}
		request_target_.path_ = path_query.first;
		request_target_.query_ = path_query.second;
	}
};
#endif
