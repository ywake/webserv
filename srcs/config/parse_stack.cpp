#include "parse_stack.hpp"

namespace conf
{
	ParseStack::ParseStack(/* args */) : head_stack_(), content_stack(), layer_(0) {}

	ParseStack::~ParseStack() {}

	bool ParseStack::push(const Header &head)
	{
		switch (layer_) {
		case kOutSide:
			if (head != "server") {
				return false;
			}
			break;
		case kServer:
			if (!head.StartWith("location")) {
				return false;
			}
			break;
		case kLocation:
		default:
			return false;
		}

		head_stack_.push(head);
		content_stack.push(Contents());
		++layer_;
		return true;
	}

	bool ParseStack::pop()
	{
		head_stack_.pop();
		content_stack.pop();
		--layer_;
		switch (layer_) {
		case kOutSide:
		case kServer:
			return true;
		case kLocation:
		default:
			return false;
		}
	}

	bool ParseStack::empty() const
	{
		return head_stack_.empty() && content_stack.empty();
	}

	ParseStack::Header ParseStack::TopHeader()
	{
		return head_stack_.top();
	}

	ParseStack::Contents ParseStack::TopContents()
	{
		return content_stack.top();
	}

	bool ParseStack::AddContent(ThinString content)
	{
		switch (layer_) {
		case kServer:
		case kLocation:
			content_stack.top().push_back(content);
			return true;
		case kOutSide:
		default:
			return false;
		}
	}
} // namespace conf
