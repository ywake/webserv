#ifndef LISTENER_HPP
#define LISTENER_HPP

#include "connection.hpp"
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
		Listener(const std::string &port);
		int        Listen();
		Connection Accept();
	};
} // namespace server

#endif
