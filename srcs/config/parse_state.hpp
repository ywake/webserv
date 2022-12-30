#ifndef PARSE_STATE_HPP
#define PARSE_STATE_HPP

#include "thin_string.hpp"

#include <set>

namespace conf
{
	class ParseState
	{
	  public:
		typedef std::set<char> Delims;

	  public:
		int layer_;
		enum layers {
			kOutSide,
			kServer,
			kLocation
		};

	  private:
		/* data */
	  public:
		ParseState(/* args */);
		~ParseState();

		void Increment(const ThinString &directive);
		void Decrement();

		bool HasContent() const;
	};

} // namespace conf

#endif
