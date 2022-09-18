#include "absolute_uri.hpp"
#include "error.hpp"
#include "parse_uri.hpp"

// TODO refactor like isObsFold
// absolute-URI  = scheme ":" hier-part [ "?" query ]
AbsoluteUri::AbsoluteUri(const ThinString &str)
{
	if (str.find(":") == ThinString::npos) {
		throw Error("400");
	}
	ThinString::ThinStrPair scheme_hier = str.DivideBy(":");
	ThinString::ThinStrPair hier_query  = scheme_hier.second.DivideBy("?");

	TrySetScheme(scheme_hier.first);
	hier_part_ = HierPart(hier_query.first);
	TrySetQuery(hier_query.second);
}

AbsoluteUri::AbsoluteUri(
	const ThinString &scheme, const HierPart &hier_part, const ThinString &query
)
	: scheme_(scheme), hier_part_(hier_part), query_(query)
{}

void AbsoluteUri::TrySetScheme(const ThinString &scheme)
{
	if (!ABNF::IsScheme(scheme)) {
		throw Error("400");
	}
	scheme_ = scheme;
}

void AbsoluteUri::TrySetQuery(const ThinString &query)
{
	if (!ABNF::IsQuery(query)) {
		throw Error("400");
	}
	query_ = query;
}

const ThinString &AbsoluteUri::GetScheme() const
{
	return scheme_;
}

const HierPart &AbsoluteUri::GetHierPart() const
{
	return hier_part_;
}

const ThinString &AbsoluteUri::GetQuery() const
{
	return query_;
}

bool AbsoluteUri::operator==(const AbsoluteUri &rhs) const
{
	if (this == &rhs) {
		return true;
	}
	return scheme_ == rhs.GetScheme() && hier_part_ == rhs.GetHierPart() &&
		   query_ == rhs.GetQuery();
}

bool AbsoluteUri::operator!=(const AbsoluteUri &rhs) const
{
	return !(rhs == *this);
}

std::ostream &operator<<(std::ostream &os, const AbsoluteUri &uri)
{
	os << "scheme: " << uri.GetScheme() << std::endl;
	os << "hier_part: " << uri.GetHierPart() << std::endl;
	os << "query: " << uri.GetQuery() << std::endl;
	return os;
}
