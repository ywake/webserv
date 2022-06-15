#ifndef STATE_HPP
#define STATE_HPP

#include "server.hpp"
#include "event.hpp"

typedef enum {
	ACCEPT = 0,
	SERVE,
	CLOSE,
	END
} State;

#endif // STATE_HPP
