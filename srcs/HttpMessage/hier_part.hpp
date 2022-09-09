#ifndef HIER_PART_HPP
#define HIER_PART_HPP

#include "ThinString.hpp"
#include "authority.hpp"

class HierPart
{
  private:
	Authority authority_;
	ThinString path_;
	void ParseAuthorityPath(ThinString hier_part);
	void TrySetPath(ThinString hier_part);

  public:
	HierPart();
	HierPart(const ThinString &hier_part);
	HierPart(const Authority &authority, const ThinString &path);
	HierPart(const HierPart &other);

	HierPart &operator=(const HierPart &other);
	bool operator==(const HierPart &rhs) const;
	bool operator!=(const HierPart &rhs) const;

	const Authority &GetAuthority() const;
	const ThinString &GetPath() const;
};

std::ostream &operator<<(std::ostream &os, const HierPart &hier_part);

#endif // HIER_PART_HPP
