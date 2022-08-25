#include "ThinString.hpp"

ThinString::ReferenceCount ThinString::reference_count_ = ThinString::ReferenceCount();

ThinString::ThinString() : base_(), start_(), length_() {}

ThinString::ThinString(const std::string &str, std::size_t start, std::size_t length)
	: start_(start), length_(length)
{
	init(str);
}

ThinString::ThinString(const char *str, std::size_t start, std::size_t length)
	: start_(start), length_(length)
{
	std::string s = str;
	init(s);
}

ThinString::ThinString(const ThinString &other, std::size_t start, std::size_t length)
{
	*this = other.substr(start, length);
}

ThinString::~ThinString()
{
	ReferenceCount::iterator it = reference_count_.find(*base_);

	if (--it->second == 0) {
		reference_count_.erase(it);
		base_ = NULL;
	}
}

void ThinString::init(const std::string &str)
{
	reference_count_[str]++;
	base_ = &reference_count_.find(str)->first;
	start_ = std::min(start_, base_->size());
	length_ = std::min(length_, base_->size() - start_);
}

std::size_t ThinString::len() const
{
	return length_;
}

std::size_t ThinString::size() const
{
	return length_;
}

std::size_t ThinString::find(const std::string &str, std::size_t pos) const
{
	std::size_t offset = std::min(pos, length_);
	const_iterator itr = std::search(begin() + offset, end(), str.begin(), str.end());
	if (itr == end()) {
		return std::string::npos;
	}
	return itr - begin();
}

std::size_t ThinString::find(const char *s, std::size_t pos) const
{
	return find(std::string(s), pos);
}

std::size_t ThinString::find(char ch, std::size_t pos) const
{
	char s[2] = {ch};
	return find(std::string(s), pos);
}

ThinString ThinString::substr(std::size_t pos, std::size_t size) const
{
	std::size_t sub_start = std::min(pos, length_);
	std::size_t sub_length = std::min(size, length_ - sub_start);
	return ThinString(*base_, start_ + sub_start, sub_length);
}

std::string ThinString::ToString() const
{
	return base_->substr(start_, len());
}

ThinString::const_iterator ThinString::begin() const
{
	return base_->begin() + start_;
}

ThinString::const_iterator ThinString::end() const
{
	return this->begin() + length_;
}

ThinString &ThinString::operator=(const ThinString &rhs)
{
	if (this == &rhs) {
		return *this;
	}
	start_ = rhs.start_;
	length_ = rhs.length_;
	base_ = rhs.base_;
	reference_count_[*base_]++;
	return *this;
}

std::ostream &operator<<(std::ostream &os, const ThinString &thin_str)
{
	os << thin_str.ToString();
	return (os);
}
