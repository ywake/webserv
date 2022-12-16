#ifndef POST_RESPONSE_HPP
#define POST_RESPONSE_HPP

#include "a_response.hpp"
#include "body_writer.hpp"
#include "location_conf.hpp"
#include "managed_fd.hpp"

namespace response
{
	class PostResponse : public AResponse
	{
	  private:
		const conf::LocationConf &location_;
		ManagedFd                 managed_fd_;
		std::string               filename_;
		server::BodyWriter        body_writer_;

	  private:
		PostResponse();
		PostResponse(const PostResponse &other);
		PostResponse &operator=(const PostResponse &rhs);

	  public:
		PostResponse(const server::IRequest &request, const conf::LocationConf &location);
		void           Perform(const event::Event &event);
		bool           HasFd() const;
		Emptiable<int> GetFd() const;

	  private:
		int TryOpen(const std::string &filename) const;
	};
} // namespace response

#endif
