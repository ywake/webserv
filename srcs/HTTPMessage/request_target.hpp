#ifndef REQUEST_TARGET_HPP
#define REQUEST_TARGET_HPP
#include "uri.hpp"

class RequestTarget
{
  public:
	enum RequestForm { ORIGIN, ABSOLUTE, AUTHORITY, ASTERISK };
	RequestForm form_type_;
	URI request_target_; // TODO: variable name

	RequestTarget() : form_type_(), request_target_() {}

	RequestTarget(RequestForm form_type, URI request_target)
		: form_type_(form_type), request_target_(request_target)
	{
	}

	bool operator==(const RequestTarget &rhs) const
	{
		return form_type_ == rhs.form_type_ && request_target_ == rhs.request_target_;
	}
};

#endif
