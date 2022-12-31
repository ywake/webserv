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

	void CgiResponse::ChildProcess()
	{
		try {
			parent_fd_.Close();
			StringArray  args = CreateArgs();
			StringArray  envs = CreateEnvs();
			Result<void> res  = utils::SetSignalHandler(SIGPIPE, SIG_IGN, 0);
			if (res.IsErr()) {
				std::cerr << res.Err() << std::endl;
				exit(1);
			}
			if (dup2(child_fd_.GetFd(), STDIN_FILENO) == -1 ||
				dup2(child_fd_.GetFd(), STDOUT_FILENO) == -1) {
				perror("dup2");
				exit(1);
			}
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

	std::vector<std::string> CgiResponse::CreateArgs()
	{
		const Emptiable<std::string> &cgi_path = location_conf_.GetCgiPath();
		std::vector<std::string>      args;
		args.push_back(cgi_path.Value());
		args.push_back(utils::JoinPath(location_conf_.GetRoot(), script_name_));
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
