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
	HierPart(const ThinString &hier_part);
};

#endif // HIER_PART_HPP
