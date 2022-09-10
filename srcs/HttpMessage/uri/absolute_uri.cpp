#include "absolute_uri.hpp"
#include "error.hpp"
#include "parse_uri.hpp"

// TODO anbf
AbsoluteUri::AbsoluteUri(const ThinString &str)
{
	ThinString::ThinStrPair scheme_hier = str.DivideBy(":");
	ThinString::ThinStrPair hier_query	= scheme_hier.second.DivideBy("?");

	TrySetScheme(scheme_hier.first);
	hier_part_ = HierPart(hier_query.first);
	TrySetQuery(hier_query.second);
}

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
