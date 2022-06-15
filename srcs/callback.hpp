#ifndef CALLBACK_HPP
#define CALLBACK_HPP

#include "event.hpp"
#include "server.hpp"
#include <vector>

Event OnAccept(int fd, Server *s);
Event OnServe(int fd, Server *s);
Event OnClose(int fd, Server *s);

#endif
