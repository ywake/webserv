#ifndef LISTENER_HPP
#define LISTENER_HPP

#include "server_conf.hpp"
// TODO fdclass
namespace server
{
	class Listener
	{
	  private:
		int               fd_;
		conf::ServerConf &configs_;

	  public:
		Listener(int fd);
		int Accept();
		int Listen();
	};
} // namespace server
#endif
