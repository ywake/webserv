#ifndef ABOSLUTE_URI_HPP
#define ABOSLUTE_URI_HPP

#include <string>

#include "ThinString.hpp"
#include "hier_part.hpp"
#include "i_target_form.hpp"

class AbsoluteUri : public ITargetForm
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

  public:
	const ThinString &GetScheme() const;
	const ThinString &GetUserinfo() const;
	const ThinString &GetHost() const;
	const ThinString &GetPort() const;
	const ThinString &GetPath() const;
	const ThinString &GetQuery() const;
};

#endif
