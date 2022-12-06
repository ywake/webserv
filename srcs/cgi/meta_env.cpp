#include "meta_env.hpp"
#include "header_section.hpp"
#include "meta_var.hpp"
#include <vector>

namespace cgi
{
	typedef HeaderSection::Values::const_iterator HeaderValuesIter;
	void AddMetaVars(const HeaderSection &headers, std::vector<MetaVar> &meta_vars);

	void SetMetaEnv(const server::IRequest &request, char *envp[])
	{
		HeaderSection        headers = request.Headers();
		std::vector<MetaVar> meta_vars;
		AddMetaVars(headers, meta_vars);
		for (std::vector<MetaVar>::const_iterator it = meta_vars.begin(); it != meta_vars.end();
			 ++it) {
			const MetaVar &var     = *it;
			std::string    env_str = var.Name() + "=" + var.Value();
			int            res     = putenv(const_cast<char *>(env_str.c_str()));
			if (res) {
				return; // TODO: error
			}
		}
	}

	void AddMetaVars(const HeaderSection &headers, std::vector<MetaVar> &meta_vars)
	{
		AddAuthType(meta_vars, headers);
	}

	void AddAuthType(std::vector<MetaVar> &meta_vars, const HeaderSection &headers)
	{
		if (headers.Contains("Authorization") == false) {
			return;
		}

		HeaderSection::Values values = headers["Authorization"];
		std::string           auth_type;

		for (HeaderValuesIter it = values.begin(); it != values.end(); ++it) {
			HeaderValue value = *it;
			auth_type += value.GetValue();
		}
		meta_vars.push_back(MetaVar("AUTH_TYPE", auth_type, false));
	}

} // namespace cgi
