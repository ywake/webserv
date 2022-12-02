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
  public:
	enum FromType {
		kOriginForm,
		kAbsoluteForm,
		kAuthorityForm,
		kAsteriskForm
	};

  private:
	FromType        form_type_;
	RequestFormData form_data_;

  public:
	RequestTarget();
	RequestTarget(const OriginForm &form);
	RequestTarget(const AbsoluteForm &form);
	RequestTarget(const AuthorityForm &form);
	RequestTarget(const AsteriskForm &form);

	RequestTarget &operator=(const RequestTarget &other);
	bool           operator==(const RequestTarget &rhs) const;
	bool           operator!=(const RequestTarget &rhs) const;

	const RequestFormData &GetRequestFormData() const;
};

std::ostream &operator<<(std::ostream &os, const RequestTarget &request_target);

#endif
