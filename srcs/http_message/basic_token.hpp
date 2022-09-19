#ifndef BASIC_TOKEN_HPP
#define BASIC_TOKEN_HPP
#include "thin_string.hpp"

template <typename TokenId>
class BasicToken
{
  private:
	ThinString s_;
	TokenId    id_;

  public:
	BasicToken() : s_(), id_() {}
	BasicToken(const ThinString &str, TokenId id) : s_(str), id_(id) {}

	TokenId GetId() const
	{
		return id_;
	}

	const ThinString &GetStr() const
	{
		return s_;
	}

	std::size_t GetLen() const
	{
		return s_.size();
	}

	bool operator==(const BasicToken &rhs) const
	{
		return id_ == rhs.id_ && s_ == rhs.s_;
	}
	bool operator!=(const BasicToken &rhs) const
	{
		return !(*this == rhs);
	}
};
#endif
