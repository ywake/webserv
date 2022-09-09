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
	HierPart &operator=(const HierPart &other);

	const Authority &GetAuthority() const;
	const ThinString &GetPath() const;
};

#endif // HIER_PART_HPP
