#include "http_exceptions.hpp"

ParseErrorException::ParseErrorException() : std::runtime_error("Bad Request") {}
