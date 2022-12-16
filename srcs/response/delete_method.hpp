#ifndef DELETE_METHOD_HPP
#define DELETE_METHOD_HPP

#include "a_response.hpp"
#include "location_conf.hpp"

namespace response
{
	class DeleteMethod : public AResponse
	{
	  private:
		const conf::LocationConf &location_;

	  private:
		DeleteMethod();
		DeleteMethod(const DeleteMethod &other);
		DeleteMethod &operator=(const DeleteMethod &rhs);

	  public:
		DeleteMethod(const server::IRequest &request, const conf::LocationConf &location);
		void           Perform(const event::Event &event);
		bool           HasFd() const;
		Emptiable<int> GetFd() const;
	};
} // namespace response

#endif
