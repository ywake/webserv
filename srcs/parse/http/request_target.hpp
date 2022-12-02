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
	enum FormType {
		kOriginForm,
		kAbsoluteForm,
		kAuthorityForm,
		kAsteriskForm
	};

  private:
	FormType        form_type_;
	RequestFormData form_data_;

  public:
	RequestTarget();
	RequestTarget(const RequestTarget &other);
	RequestTarget(const OriginForm &form);
	RequestTarget(const AbsoluteForm &form);
	RequestTarget(const AuthorityForm &form);
	RequestTarget(const AsteriskForm &form);
	RequestTarget(const RequestFormData &request_target, FormType form_type);

	RequestTarget &operator=(const RequestTarget &other);
	bool           operator==(const RequestTarget &rhs) const;
	bool           operator!=(const RequestTarget &rhs) const;

	const RequestFormData &GetRequestFormData() const;
	FormType               GetFormType() const;
};

std::ostream &operator<<(std::ostream &os, const RequestTarget &request_target);

#endif
