#include "ThinString.hpp"

bool IsOverFlowAdd(std::size_t a, std::size_t b)
{
	return a > ~0 - b;
}

ThinString::ReferenceCount ThinString::reference_count_ = ThinString::ReferenceCount();

ThinString::ThinString() : content_(), start_(), length_() {}

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
	ReferenceCount::iterator it = reference_count_.find(*content_);

	if (--it->second == 0) {
		reference_count_.erase(it);
		content_ = NULL;
	}
}

void ThinString::init(const std::string &str)
{
	reference_count_[str]++;
	content_ = &reference_count_.find(str)->first;
	start_ = std::min(start_, content_->size());
	length_ = std::min(length_, content_->size() - start_);
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
	pos = std::min(pos, length_);
	return content_->find(str.c_str(), start_ + pos, length_);
}

std::size_t ThinString::find(const char *s, std::size_t pos, std::size_t count) const
{
	pos = std::min(pos, length_);
	return content_->find(s, start_ + pos, std::min(count, length_));
}

std::size_t ThinString::find(const char *s, std::size_t pos) const
{
	pos = std::min(pos, length_);
	return content_->find(s, start_ + pos, length_);
}

std::size_t ThinString::find(char ch, std::size_t pos) const
{
	pos = std::min(pos, length_);
	char s[2] = {ch};
	return content_->find(s, start_ + pos, length_);
}

ThinString ThinString::substr(std::size_t pos, std::size_t size) const
{
	std::size_t sub_start = std::min(pos, length_);
	std::size_t sub_length = std::min(size, length_ - sub_start);
	return ThinString(*content_, sub_start, sub_length);
}

std::string ThinString::ToString() const
{
	return content_->substr(start_, len());
}

ThinString::const_iterator ThinString::begin() const
{
	return content_->begin() + start_;
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
	content_ = rhs.content_;
	reference_count_[*content_]++;
	return *this;
}

std::ostream &operator<<(std::ostream &os, const ThinString &thin_str)
{
	os << thin_str.ToString();
	return (os);
}