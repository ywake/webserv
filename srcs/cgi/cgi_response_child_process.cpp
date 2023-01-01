#include <cerrno>
#include <cstdio>

#include "cgi_response.hpp"
#include "debug.hpp"
#include "meta_env.hpp"
#include "string_array.hpp"
#include "webserv_utils.hpp"

extern char **environ;

namespace cgi
{
	void CgiResponse::ExecChild(const std::string &script_name, ManagedFd &child_fd)
	{
		try {
			log("child process");
			parent_fd_.Close();
			Result<void> res = utils::SetSignalHandler(SIGPIPE, SIG_IGN, 0);
			if (res.IsErr()) {
				log("cgi", res.Err());
				exit(1);
			}
			// TODO chdir;
			if (dup2(child_fd.GetFd(), STDIN_FILENO) == -1 ||
				dup2(child_fd.GetFd(), STDOUT_FILENO) == -1) {
				perror("dup2");
				exit(1);
			}
			StringArray args = CreateArgs(
				location_conf_.GetCgiPath().Value(),
				script_name,
				request_.GetRequestLine().GetRequestTarget().GetRequestFormData().query_
			);
			StringArray envs = CreateEnvs();
			execve(
				location_conf_.GetCgiPath().Value().c_str(),
				const_cast<char *const *>(args.CArray()),
				const_cast<char *const *>(envs.CArray())
			);
		} catch (std::exception &e) {
			e.what();
		} catch (...) {
			log("unknown exceptionin cgi child");
		}
		exit(1);
	}

	std::vector<std::string> CgiResponse::CreateArgs(
		const std::string &cgi_path, const std::string &script_name, const std::string &query
	)
	{
		std::vector<std::string> args;

		args.push_back(cgi_path);
		args.push_back(utils::JoinPath(location_conf_.GetRoot(), script_name));
		(void)query; // TODO query
		return args;
	}

	std::vector<std::string> CgiResponse::CreateEnvs()
	{
		std::vector<std::string> envs;
		for (size_t i = 0; environ[i] != NULL; ++i) {
			envs.push_back(environ[i]);
		}
		// SetMetaEnv(envs);
		return envs;
	}

	void CgiResponse::SetMetaEnv(std::vector<const char *> &envs)
	{
		SetAuthType(envs, request_);
		SetContentLength(envs, request_);
		SetContentType(envs, request_);
	}

} // namespace cgi
