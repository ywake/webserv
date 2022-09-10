#ifndef REQUEST_TARGET_HPP
#define REQUEST_TARGET_HPP

#include "i_target_form.hpp"
#include "request_form_data.hpp"

class RequestTarget
{
  public:
	enum RequestForm {
		ORIGIN,
		ABSOLUTE,
		AUTHORITY,
		ASTERISK,
		UNDEFINED
	};

  private:
	RequestForm form_type_;
	/**
	 * Asterisk-formのみURI形式にパースできないので、
	 * Asterisk-formはform_typeで特定する。
	 */
	RequestFormData form_data_;

  public:
	RequestTarget();
	RequestTarget(std::string uri);
	RequestTarget(RequestForm form_type, RequestFormData request_target);

	RequestTarget			  &operator=(const RequestTarget &other);
	bool					   operator==(const RequestTarget &rhs) const;
	bool					   operator!=(const RequestTarget &rhs) const;
	RequestTarget::RequestForm SpecifyForm(const std::string &uri);
	void					   SetFormData(const ITargetForm *uri);

	RequestForm GetRequestForm() const;
};

std::ostream &operator<<(std::ostream &os, const RequestTarget &request_target);

#endif
