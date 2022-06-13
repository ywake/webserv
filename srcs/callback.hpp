#ifndef CALLBACK_HPP
#define CALLBACK_HPP

#include "event.hpp"
#include "server.hpp"
#include <vector>

std::vector<Event> OnAccept(int fd, Server *s);
std::vector<Event> OnServe(int fd, Server *s);

#endif
