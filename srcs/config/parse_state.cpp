#include "parse_state.hpp"

#include "config_exceptions.hpp"

namespace conf
{

	ParseState::ParseState(/* args */) : layer_(0) {}

	ParseState::~ParseState() {}

	void ParseState::Increment(const ThinString &directive)
	{
		switch (layer_) {
		case kOutSide:
			if (directive != "server") {
				throw conf::ConfigException("Invalid config");
			}
			break;
		case kServer:
			if (!directive.StartWith("location ")) {
				throw conf::ConfigException("Invalid config");
			}
			break;
		case kLocation:
		default:
			throw conf::ConfigException("Invalid config");
		}
		++layer_;
	}

	void ParseState::Decrement()
	{
		switch (layer_) {
		case kServer:
		case kLocation:
			break;
		case kOutSide:
		default:
			throw conf::ConfigException("Invalid config");
		}
		--layer_;
	}

	bool ParseState::HasContent() const
	{
		switch (layer_) {
		case kServer:
		case kLocation:
			return true;
		default:
			return false;
		}
	}
} // namespace conf
