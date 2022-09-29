#include "hier_part.hpp"
#include "http_exceptions.hpp"
#include "parse_path.hpp"

static const char  *k2slash     = "//";
static const size_t k2slashSize = sizeof("//") - 1;

HierPart::HierPart() : authority_(), path_() {}

// hier-part    = "//" authority path-abempty
//              / path-absolute
//              / path-rootless
//              / path-empty
HierPart::HierPart(const ThinString &hier_part) : authority_()
{
	if (hier_part.substr(0, k2slashSize) == k2slash) {
		ParseAuthorityPath(hier_part);
	} else {
		ParsePath(hier_part);
	}
}

HierPart::HierPart(const Authority &authority, const ThinString &path)
	: authority_(authority), path_(path)
{}

HierPart::HierPart(const HierPart &other) : authority_(other.authority_), path_(other.path_) {}

void HierPart::ParseAuthorityPath(const ThinString &hier_part)
{
	size_t                  authority_start_idx = k2slashSize;
	ThinString              after_2slash        = hier_part.substr(authority_start_idx);
	ThinString::ThinStrPair authority_path =
		after_2slash.DivideBy("/", ThinString::kKeepDelimRight);
	authority_ = Authority(authority_path.first);
	if (!ABNF::IsPathAbempty(authority_path.second)) {
		throw BadRequestException();
	}
	path_ = authority_path.second;
}

void HierPart::ParsePath(const ThinString &hier_part)
{
	if (ABNF::IsPathAbempty(hier_part) || ABNF::IsPathRootless(hier_part)) {
		path_ = hier_part;
	} else {
		throw BadRequestException();
	}
}

HierPart &HierPart::operator=(const HierPart &other)
{
	if (this == &other) {
		return *this;
	}
	authority_ = other.authority_;
	path_      = other.path_;
	return *this;
}

bool HierPart::operator==(const HierPart &other) const
{
	if (this == &other) {
		return true;
	}
	return authority_ == other.authority_ && path_ == other.path_;
}

bool HierPart::operator!=(const HierPart &other) const
{
	return !(*this == other);
}

const Authority &HierPart::GetAuthority() const
{
	return authority_;
}

const ThinString &HierPart::GetPath() const
{
	return path_;
}

std::ostream &operator<<(std::ostream &os, const HierPart &hier_part)
{
	os << hier_part.GetAuthority() << std::endl;
	os << "path :" << hier_part.GetPath() << std::endl;
	return os;
}
