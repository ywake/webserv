#ifndef PARSE_STACK_HPP
#define PARSE_STACK_HPP

#include "thin_string.hpp"

#include <stack>
#include <vector>

namespace conf
{

	class ParseStack
	{
	  public:
		typedef ThinString              Header;
		typedef std::vector<ThinString> Contents;

	  private:
		std::stack<Header>   head_stack_;
		std::stack<Contents> content_stack;
		int                  layer_;
		enum layers {
			kOutSide,
			kServer,
			kLocation
		};

	  public:
		ParseStack(/* args */);
		~ParseStack();

		void push(const Header &head);
		void pop();
		bool empty() const;

		Header   &TopHeader();
		Contents &TopContents();
		void      AddContent(ThinString content);
	};
} // namespace conf

#endif
