#ifndef LISTENER_HPP
#define LISTENER_HPP

#include "virtual_server_confs.hpp"
// TODO fdclass
namespace server
{
	class Listener
	{
	  private:
		int                       fd_;
		conf::VirtualServerConfs &configs_;

	  public:
		Listener(int fd);
		int Accept();
		int Listen();
	};
} // namespace server
#endif
