#include "parse_stack.hpp"
#include "config_exceptions.hpp"

namespace conf
{
	ParseStack::ParseStack(/* args */) : head_stack_(), content_stack(), layer_(0) {}

	ParseStack::~ParseStack() {}

	void ParseStack::push(const Header &head)
	{
		switch (layer_) {
		case kOutSide:
			if (head != "server") {
				throw conf::ConfigException("Invalid config: push(kOutSide)");
			}
			break;
		case kServer:
			if (!head.StartWith("location ")) {
				throw conf::ConfigException("Invalid config: push(kServer)");
			}
			break;
		case kLocation:
		default:
			throw conf::ConfigException("Invalid config: push(other)");
		}

		head_stack_.push(head);
		content_stack.push(Contents());
		++layer_;
	}

	void ParseStack::pop()
	{
		head_stack_.pop();
		content_stack.pop();
		--layer_;
		switch (layer_) {
		case kOutSide:
		case kServer:
			break;
		case kLocation:
		default:
			throw conf::ConfigException("Invalid config: pop");
		}
	}

	bool ParseStack::empty() const
	{
		return head_stack_.empty() && content_stack.empty();
	}

	ParseStack::Header &ParseStack::TopHeader()
	{
		return head_stack_.top();
	}

	ParseStack::Contents &ParseStack::TopContents()
	{
		return content_stack.top();
	}

	void ParseStack::AddContent(ThinString content)
	{
		switch (layer_) {
		case kServer:
		case kLocation:
			content_stack.top().push_back(content);
			break;
		case kOutSide:
		default:
			throw conf::ConfigException("Invalid config: AddContent");
		}
	}
} // namespace conf
