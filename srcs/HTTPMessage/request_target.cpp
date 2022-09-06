#include "request_target.hpp"

std::ostream &operator<<(std::ostream &os, const RequestTarget &request_target)
{
	switch (request_target.form_type_) {
	case RequestTarget::UNDEFINED:
		os << "UNDEFINED";
		break;
	case RequestTarget::ORIGIN:
		os << "ORIGIN";
		break;
	case RequestTarget::ABSOLUTE:
		os << "ABSOLUTE";
		break;
	case RequestTarget::AUTHORITY:
		os << "AUTHORITY";
		break;
	case RequestTarget::ASTERISK:
		os << "ASTERISK";
		break;
	}
	os << "\n" << request_target.request_target_;
	return os;
}
