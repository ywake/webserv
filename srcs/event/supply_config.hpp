#ifndef SUPPLY_CONFIG_HPP
#define SUPPLY_CONFIG_HPP

#include "i_event.hpp"
#include "server_types.hpp"

namespace event
{
	class SupplyConfig : public IEvent
	{
	  private:
		const server::Connections &connections_;
		const server::Configs     &configs_;

	  private:
		SupplyConfig();

	  public:
		SupplyConfig(server::Connections &connections, const server::Configs &configs);
		void Run();
	};
} // namespace event
#endif
