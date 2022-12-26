#ifndef POST_METHOD_HPP
#define POST_METHOD_HPP

#include "a_response.hpp"
#include "body_writer.hpp"
#include "location_conf.hpp"
#include "managed_fd.hpp"

namespace response
{
	class PostMethod : public AResponse
	{
	  private:
		const conf::LocationConf &location_;
		ManagedFd                 managed_fd_;
		std::string               filename_;
		server::BodyWriter        body_writer_;

	  private:
		PostMethod();
		PostMethod(const PostMethod &other);
		PostMethod &operator=(const PostMethod &rhs);

	  public:
		PostMethod(const server::IRequest &request, const conf::LocationConf &location);
		void           Perform(const event::Event &event);
		bool           HasFd() const;
		Emptiable<int> GetFd() const;

	  private:
		int         TryOpen(const std::string &filename) const;
		std::string CreateLocationUrl(const std::string &path);
	};
} // namespace response

#endif
