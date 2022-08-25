#include "ThinString.hpp"

std::size_t CapNumberAt(std::size_t num, std::size_t limit)
{
	if (num > limit) {
		num = limit;
	}
	return num;
}

bool IsOverFlowAdd(std::size_t a, std::size_t b)
{
	return a > ~0 - b;
}

ThinString::ReferenceCount ThinString::reference_count_ = ThinString::ReferenceCount();

ThinString::ThinString() : content_(), start_(), end_() {}

ThinString::ThinString(const std::string &str, std::size_t start, std::size_t end)
	: start_(start), end_(end)
{
	init(str);
}

ThinString::ThinString(const char *str, std::size_t start, std::size_t end)
	: start_(start), end_(end)
{
	std::string s = str;
	init(s);
}

ThinString::ThinString(const ThinString &other, std::size_t start, std::size_t end)
{
	*this = other.substr(start, end);
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
	start_ = CapNumberAt(start_, content_->size());
	end_ = CapNumberAt(end_, content_->size());
}

std::size_t ThinString::len() const
{
	return end_ - start_;
}

std::size_t ThinString::size() const
{
	return end_ - start_;
}

std::size_t ThinString::find(const std::string &str, std::size_t pos) const
{
	if (IsOverFlowAdd(pos, start_)) {
		return std::string::npos;
	}
	std::size_t start_pos = CapNumberAt(start_ + pos, end_);
	return content_->find(str.c_str(), start_pos, end_);
}

std::size_t ThinString::find(const char *s, std::size_t pos, std::size_t count) const
{
	if (IsOverFlowAdd(pos, start_)) {
		return std::string::npos;
	}
	std::size_t start_pos = CapNumberAt(start_ + pos, end_);
	return content_->find(s, start_pos, CapNumberAt(count, end_ - start_pos));
}

std::size_t ThinString::find(const char *s, std::size_t pos) const
{
	if (IsOverFlowAdd(pos, start_)) {
		return std::string::npos;
	}
	std::size_t start_pos = CapNumberAt(start_ + pos, end_);
	return content_->find(s, start_pos, end_);
}

std::size_t ThinString::find(char ch, std::size_t pos) const
{
	if (IsOverFlowAdd(pos, start_)) {
		return std::string::npos;
	}
	std::size_t start_pos = CapNumberAt(start_ + pos, end_);
	char s[2] = {ch};
	return content_->find(s, start_pos, end_);
}

ThinString ThinString::substr(std::size_t pos, std::size_t size) const
{
	if (IsOverFlowAdd(pos, start_)) {
		pos = 0;
	}
	std::size_t sub_start = CapNumberAt(start_ + pos, this->len());
	size = CapNumberAt(size, end_ - sub_start);
	std::size_t sub_end = sub_start + size;
	return ThinString(*content_, sub_start, sub_end);
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
	return content_->begin() + end_;
}

ThinString &ThinString::operator=(const ThinString &rhs)
{
	if (this == &rhs) {
		return *this;
	}
	start_ = rhs.start_;
	end_ = rhs.end_;
	content_ = rhs.content_;
	reference_count_[*content_]++;
	return *this;
}

std::ostream &operator<<(std::ostream &os, const ThinString &thin_str)
{
	os << thin_str.ToString();
	return (os);
}