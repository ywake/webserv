#include "origin_form.hpp"
#include "http_exceptions.hpp"
#include "parse_path.hpp"
#include "parse_uri.hpp"

static const ThinString kEmpty = "";

OriginForm::OriginForm() : path_(), query_() {}

// origin-form = absolute-path [ "?" query ]
OriginForm::OriginForm(const ThinString &str)
{
	ThinString::ThinStrPair path_query = str.DivideBy("?");

	if (!ABNF::IsPathAbsolute(path_query.first) || !ABNF::IsQuery(path_query.second)) {
		throw BadRequestException();
	}
	path_  = path_query.first;
	query_ = path_query.second;
}

OriginForm::OriginForm(const OriginForm &other) : path_(other.path_), query_(other.query_) {}

OriginForm::OriginForm(const ThinString &path, const ThinString &query) : path_(path), query_(query)
{}

OriginForm &OriginForm::operator=(const OriginForm &other)
{
	if (this == &other) {
		return *this;
	}
	path_  = other.path_;
	query_ = other.query_;
	return *this;
}

bool OriginForm::operator==(const OriginForm &other) const
{
	if (this == &other) {
		return true;
	}
	return path_ == other.path_ && query_ == other.query_;
}

bool OriginForm::operator!=(const OriginForm &other) const
{
	return !(*this == other);
}

const ThinString &OriginForm::GetScheme() const
{
	return kEmpty;
}

const ThinString &OriginForm::GetUserinfo() const
{
	return kEmpty;
}

const ThinString &OriginForm::GetHost() const
{
	return kEmpty;
}

const ThinString &OriginForm::GetPort() const
{
	return kEmpty;
}

const ThinString &OriginForm::GetPath() const
{
	return path_;
}

const ThinString &OriginForm::GetQuery() const
{
	return query_;
}

std::ostream &operator<<(std::ostream &os, const OriginForm &origin)
{
	os << origin.GetPath() << std::endl;
	os << origin.GetQuery() << std::endl;
	return os;
}
