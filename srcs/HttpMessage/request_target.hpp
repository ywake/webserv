#ifndef REQUEST_TARGET_HPP
#define REQUEST_TARGET_HPP
#include "ThinString.hpp"
#include "i_target_form.hpp"
#include "parse_path.hpp"
#include "parse_uri.hpp"
#include "result.hpp"
#include "uri.hpp"
#include <cstdlib>
#include <vector>

class RequestTarget // TODO: Abstruct説
{
  public:
	enum RequestForm { ORIGIN, ABSOLUTE, AUTHORITY, ASTERISK, UNDEFINED };
	RequestForm form_type_;
	Uri uri_;
	/**
	 * Asterisk-formのみURI形式にパースできないので、
	 * Asterisk-formはform_typeで特定する。
	 */

	RequestTarget();
	RequestTarget(std::string uri);
	RequestTarget(RequestForm form_type, Uri request_target);

	bool operator==(const RequestTarget &rhs) const;
	bool operator!=(const RequestTarget &rhs) const;
	RequestTarget::RequestForm SpecifyForm(const std::string &uri);
	void ParseOriginForm(std::string uri);
	void ParseAbsoluteForm(ThinString uri);
	void SetUri(ITargetForm *uri);
};

std::ostream &operator<<(std::ostream &os, const RequestTarget &request_target);

#endif
