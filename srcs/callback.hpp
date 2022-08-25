#ifndef CALLBACK_HPP
#define CALLBACK_HPP

#include "event_result.hpp"
#include "server.hpp"
#include <vector>

typedef EventResult (*Callback)(int fd, Server *s);

EventResult OnAccept(int fd, Server *s);
EventResult OnServe(int fd, Server *s);
EventResult OnClose(int fd, Server *s);

#endif
