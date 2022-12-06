#ifndef META_ENV_HPP
#define META_ENV_HPP

#include "i_request.hpp"

namespace cgi
{
	void SetMetaEnv(const server::IRequest &request, char *envp[]);

} // namespace cgi

#endif
