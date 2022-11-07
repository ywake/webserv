#include "response_message.hpp"
#include <cerrno>
#include <cstring>
#include <stdexcept>

using cgi::ResponseMessage;

ResponseMessage::ResponseMessage() : header_fields_(), message_body_() {}
