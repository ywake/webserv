#ifndef ABSOLUTE_URI_HPP
#define ABSOLUTE_URI_HPP

#include <string>

#include "hier_part.hpp"
#include "i_target_form.hpp"
#include "thin_string.hpp"

class AbsoluteUri
{
  private:
	ThinString scheme_;
	HierPart   hier_part_;
	ThinString query_;

  public:
	AbsoluteUri(const ThinString &str);
	AbsoluteUri(const ThinString &scheme, const HierPart &hier_part, const ThinString &query);

  private:
	void TrySetScheme(const ThinString &scheme);
	void TrySetQuery(const ThinString &query);

  public:
	const ThinString &GetScheme() const;
	const HierPart   &GetHierPart() const;
	const ThinString &GetQuery() const;

	bool operator==(const AbsoluteUri &rhs) const;
	bool operator!=(const AbsoluteUri &rhs) const;
};

std::ostream &operator<<(std::ostream &os, const AbsoluteUri &uri);

#endif
