#ifndef HIER_PART_HPP
#define HIER_PART_HPP

#include "ThinString.hpp"
#include "authority.hpp"

class HierPart
{
private:
	Authority authority_;
	std::string path_;
public:
	HierPart(ThinString hier_part);
};

#endif // HIER_PART_HPP
