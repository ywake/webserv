#ifndef REDIRECT_HPP
#define REDIRECT_HPP

#include "a_response.hpp"
#include "emptiable.hpp"
#include "location_conf.hpp"
#include "managed_fd.hpp"

namespace response
{
	class Redirect : public AResponse
	{
	  private:
		const conf::LocationConf &location_;

	  private:
		Redirect();
		Redirect(const Redirect &other);
		Redirect &operator=(const Redirect &rhs);

	  public:
		Redirect(const server::IRequest &request, const conf::LocationConf &location);
		FinEventType   Perform(const event::Event &event);
		bool           HasFd() const;
		Emptiable<int> GetFd() const;
	};
} // namespace response

#endif
