#ifndef REQUEST_TARGET_HPP
#define REQUEST_TARGET_HPP
#include "ThinString.hpp"
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
	URI uri_;
	/**
	 * Asterisk-formのみURI形式にパースできないので、
	 * Asterisk-formはform_typeで特定する。
	 */

	RequestTarget();
	RequestTarget(std::string uri);
	RequestTarget(RequestForm form_type, URI request_target);

	bool operator==(const RequestTarget &rhs) const;
	bool operator!=(const RequestTarget &rhs) const;
	void SpecifyForm(const std::string &uri);
	void ParseOriginForm(std::string uri);
	void ParseAbsoluteForm(ThinString uri);
};

std::ostream &operator<<(std::ostream &os, const RequestTarget &request_target);

#endif
