#include "hier_part.hpp"
#include "error.hpp"
#include "parse_path.hpp"

HierPart::HierPart() : authority_(), path_() {}

HierPart::HierPart(const ThinString &hier_part) : authority_()
{
	if (hier_part.substr(0, 2) == "//") {
		ParseAuthorityPath(hier_part);
	} else {
		TrySetPath(hier_part);
	}
}

void HierPart::ParseAuthorityPath(ThinString hier_part)
{
	ThinString after_2slash = hier_part.substr(2);
	ThinString::ThinStrPair authority_path =
		after_2slash.DivideBy("/", ThinString::kKeepDelimRight);
	authority_ = Authority(authority_path.first);
	TrySetPath(authority_path.second);
}

void HierPart::TrySetPath(ThinString hier_part)
{
	if (!ABNF::IsPathAbempty(hier_part)) {
		throw Error("400");
	}
	path_ = hier_part;
}

HierPart &HierPart::operator=(const HierPart &other)
{
	if (this == &other) {
		return *this;
	}
	authority_ = other.authority_;
	path_ = other.path_;
	return *this;
}

const Authority &HierPart::GetAuthority() const
{
	return authority_;
}

const ThinString &HierPart::GetPath() const
{
	return path_;
}
