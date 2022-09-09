#ifndef ABOSLUTE_URI_HPP
#define ABOSLUTE_URI_HPP

#include <string>

#include "hier_part.hpp"
#include "thin_string.hpp"

class AbsoluteUri
{
  private:
	ThinString scheme_;
	HierPart hier_part_;
	ThinString query_;

  public:
	AbsoluteUri(const ThinString &str);

  private:
	void TrySetScheme(const ThinString &scheme);
	void TrySetQuery(const ThinString &query);
};

#endif
