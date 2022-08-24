#include "ThinString.hpp"

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

ThinString::ThinString(const ThinString &other)
{
	*this = other;
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
	if (start_ > str.size()) {
		start_ = str.size();
	}
	if (end_ > str.size()) {
		end_ = str.size();
	}
	reference_count_[str]++;
	content_ = &reference_count_.find(str)->first;
}

std::size_t ThinString::len() const
{
	return end_ - start_;
}

std::string ThinString::ToString() const
{
	return (*content_).substr(start_, end_);
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
