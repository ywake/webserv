#include "ThinString.hpp"

std::size_t CapNumberAt(std::size_t num, std::size_t limit)
{
	if (num > limit) {
		num = limit;
	}
	return num;
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
	*this = other.Substr(start, end);
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

ThinString ThinString::Substr(std::size_t pos, std::size_t size) const
{
	if (pos > std::string::npos - start_) {
		pos = 0;
	}
	std::size_t sub_start = CapNumberAt(start_ + pos, this->len());
	size = CapNumberAt(size, end_ - sub_start);
	std::size_t sub_end = sub_start + size;
	return ThinString(*content_, sub_start, sub_end);
}

std::string ThinString::ToString() const
{
	return content_->substr(start_, end_);
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
