#include "parse_stack.hpp"

namespace conf
{
	ParseStack::ParseStack(/* args */) : head_stack_(), content_stack() {}

	ParseStack::~ParseStack() {}

	void ParseStack::push(const Header &head)
	{
		head_stack_.push(head);
		content_stack.push(Contents());
	}

	void ParseStack::pop()
	{
		head_stack_.pop();
		content_stack.pop();
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

	void ParseStack::AddContent(ThinString content)
	{
		content_stack.top().push_back(content);
	}
} // namespace conf
