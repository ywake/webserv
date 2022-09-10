#ifndef REQUEST_TARGET_HPP
#define REQUEST_TARGET_HPP

#include "absolute_form.hpp"
#include "asterisk_form.hpp"
#include "authority_form.hpp"
#include "i_target_form.hpp"
#include "origin_form.hpp"
#include "request_form_data.hpp"

class RequestTarget
{
  private:
	RequestFormData form_data_;

  public:
	RequestTarget();
	RequestTarget(const ITargetForm &form);
	RequestTarget(std::string uri);
	RequestTarget(RequestFormData request_target);

	RequestTarget &operator=(const RequestTarget &other);
	bool		   operator==(const RequestTarget &rhs) const;
	bool		   operator!=(const RequestTarget &rhs) const;
};

std::ostream &operator<<(std::ostream &os, const RequestTarget &request_target);

#endif
