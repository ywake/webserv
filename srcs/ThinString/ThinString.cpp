#include "ThinString.hpp"

#include <cstring>

ThinString::ReferenceCount ThinString::reference_count_ = ThinString::ReferenceCount();

ThinString::ThinString() : base_(), start_(0), length_(0)
{
	init("");
}

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
bool ThinString::empty() const
{
	return size() == 0;
}

std::size_t ThinString::len() const
{
	return length_;
}

std::size_t ThinString::size() const
{
	return length_;
}

const char &ThinString::at(size_t n) const
{
	bool is_within_range = len() > n && ~0UL - start_ > n;
	if (is_within_range) {
		return base_->at(start_ + n);

	} else {
		return base_->at(base_->length());
	}
}

const char &ThinString::back() const
{
	return this->at(this->len() - 1);
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
	std::size_t offset = std::min(pos, length_);
	std::size_t sub_length = std::min(size, length_ - offset);
	return ThinString(*base_, start_ + offset, sub_length);
}

std::string ThinString::ToString() const
{
	return base_->substr(start_, len());
}

ThinString
ThinString::CreateLeftSide(const std::string &delim, std::size_t size, DelimSide delim_side) const
{
	if (delim_side == kIncludeLeft) {
		size += delim.size();
	}
	return substr(0, size);
}

ThinString
ThinString::CreateRightSide(const std::string &delim, std::size_t start, DelimSide delim_side) const
{
	if (delim_side != kIncludeRight) {
		start += delim.size();
	}
	return substr(start);
}

ThinString::ThinStrPair ThinString::DivideBy(const std::string &delim, DelimSide delim_side) const
{
	std::size_t delim_pos = find(delim);
	bool has_no_second = delim.empty() || delim_pos == std::string::npos;
	if (has_no_second) {
		return ThinStrPair(*this, "");
	}
	ThinString left = CreateLeftSide(delim, delim_pos, delim_side);
	ThinString right = CreateRightSide(delim, delim_pos, delim_side);
	return ThinStrPair(left, right);
}

std::size_t ThinString::MeasureUntil(const std::string &delim) const
{
	std::size_t pos = find(delim);
	return pos == std::string::npos ? size() : pos;
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

bool ThinString::operator==(const ThinString &rhs) const
{
	return len() == rhs.len() && std::equal(begin(), end(), rhs.begin());
}

bool ThinString::operator==(const char *rhs) const
{
	return len() == std::strlen(rhs) && std::equal(begin(), end(), rhs);
}

bool ThinString::operator==(const std::string &rhs) const
{
	return len() == rhs.length() && std::equal(begin(), end(), rhs.begin());
}

bool ThinString::operator!=(const ThinString &rhs) const
{
	return !(*this == rhs);
}

bool ThinString::operator!=(const char *rhs) const
{
	return !(*this == rhs);
}

bool ThinString::operator!=(const std::string &rhs) const
{
	return !(*this == rhs);
}

std::ostream &operator<<(std::ostream &os, const ThinString &thin_str)
{
	os << thin_str.ToString();
	return (os);
}
