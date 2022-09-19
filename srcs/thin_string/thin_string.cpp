#include "thin_string.hpp"
#include "static_initializer.hpp"

#include <cstring>

ThinString::ReferenceCount *ThinString::reference_count_;
ThinString::StringSet      *ThinString::base_set_;

ThinString::ThinString() : base_(), start_(0), length_(0)
{
	Init("");
}

ThinString::ThinString(const std::string &str, std::size_t start, std::size_t length)
	: start_(start), length_(length)
{
	Init(str);
}

ThinString::ThinString(const char *str, std::size_t start, std::size_t length)
	: start_(start), length_(length)
{
	if (str == NULL) {
		Init("");
	} else {
		Init(str);
	}
}

ThinString::ThinString(const ThinString &other, std::size_t start, std::size_t length)
{
	*this = other.substr(start, length);
}

ThinString::ThinString(const std::string *base, std::size_t start, std::size_t length)
	: base_(base), start_(start), length_(length)
{
	(*reference_count_)[base_]++;
}

ThinString::~ThinString()
{
	ReferenceCount::iterator ref_it = reference_count_->find(base_);

	if (--ref_it->second == 0) {
		StringSet::iterator set_itr = base_set_->find(*base_);
		if (set_itr != base_set_->end()) {
			base_set_->erase(set_itr);
		}
		reference_count_->erase(ref_it);
		base_ = NULL;
	}
}

void ThinString::Init(const std::string &str)
{
	static const StaticInitializer<StringSet>      InitBaseSet(&base_set_);
	static const StaticInitializer<ReferenceCount> InitReferenceCount(&reference_count_);

	StringSet::iterator node = base_set_->insert(str).first;
	base_                    = &*node;
	(*reference_count_)[base_]++;
	start_  = std::min(start_, base_->size());
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
		return base_->at(base_->length()); // exception
	}
}

const char &ThinString::back() const
{
	if (this->empty()) {
		return this->at(0); // exception
	}
	return this->at(this->len() - 1);
}

std::size_t ThinString::find(const std::string &str, std::size_t pos) const
{
	std::size_t    offset = std::min(pos, length_);
	const_iterator itr    = std::search(begin() + offset, end(), str.begin(), str.end());
	if (itr == end()) {
		return ThinString::npos;
	}
	return itr - begin();
}

std::size_t ThinString::find(const char *s, std::size_t pos) const
{
	return find(std::string(s), pos);
}

std::size_t ThinString::find(char ch, std::size_t pos) const
{
	return find(std::string(&ch, 1), pos);
}

std::size_t ThinString::FindAfter(const std::string &str, std::size_t start_pos) const
{
	std::size_t found_pos = substr(start_pos).find(str);
	if (found_pos == ThinString::npos) {
		return ThinString::npos;
	}
	return found_pos + start_pos;
}

std::size_t ThinString::FindAfter(const char *s, std::size_t start_pos) const
{
	return FindAfter(std::string(s), start_pos);
}

std::size_t ThinString::FindAfter(char ch, std::size_t start_pos) const
{
	return FindAfter(std::string(&ch, 1), start_pos);
}

std::size_t ThinString::FindNotOf(const std::string &char_set, std::size_t pos) const
{
	std::size_t offset = std::min(pos, length_);
	for (const_iterator itr = begin() + offset; itr != end(); itr++) {
		if (char_set.find(*itr) == std::string::npos) {
			return itr - begin();
		}
	}
	return ThinString::npos;
}

ThinString ThinString::substr(std::size_t pos, std::size_t size) const
{
	std::size_t offset     = std::min(pos, length_);
	std::size_t sub_length = std::min(size, length_ - offset);
	return ThinString(base_, start_ + offset, sub_length);
}

std::string ThinString::ToString() const
{
	return base_->substr(start_, len());
}

ThinString ThinString::CreateLeftSide(
	const std::string &delim, std::size_t size, unsigned int delim_flag
) const
{
	if (delim_flag & kKeepDelimLeft) {
		size += delim.size();
	}
	return substr(0, size);
}

ThinString ThinString::CreateRightSide(
	const std::string &delim, std::size_t start, unsigned int delim_flag
) const
{
	if (!(delim_flag & kKeepDelimRight)) {
		start += delim.size();
	}
	return substr(start);
}

ThinString::ThinStrPair
ThinString::DivideBy(const std::string &delim, unsigned int delim_flag) const
{
	std::size_t delim_pos     = find(delim);
	bool        has_no_second = delim.empty() || delim_pos == ThinString::npos;
	if (has_no_second) {
		if (delim_flag & kAlignRight) {
			return ThinStrPair("", *this);
		} else {
			return ThinStrPair(*this, "");
		}
	}
	ThinString left  = CreateLeftSide(delim, delim_pos, delim_flag);
	ThinString right = CreateRightSide(delim, delim_pos, delim_flag);
	return ThinStrPair(left, right);
}

std::size_t ThinString::MeasureUntil(const std::string &delim) const
{
	std::size_t pos = find(delim);
	return pos == ThinString::npos ? size() : pos;
}

std::size_t ThinString::MeasureUntilNotOf(const std::string &char_set) const
{
	std::size_t pos = FindNotOf(char_set);
	return pos == ThinString::npos ? size() : pos;
}

// TODO test
bool ThinString::EndWith(const std::string &delim) const
{
	if (delim.size() > this->size()) {
		return false;
	}
	std::size_t start = this->size() - delim.size();
	return this->substr(start) == delim;
}

bool ThinString::StartWith(const std::string &delim) const
{
	if (delim.size() > this->size()) {
		return false;
	}
	return this->substr(0, delim.size()) == delim;
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
	start_  = rhs.start_;
	length_ = rhs.length_;
	base_   = rhs.base_;
	(*reference_count_)[base_]++;
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
