#include <cerrno>
#include <cstdio>

#include "cgi_response.hpp"
#include "debug.hpp"
#include "meta_env.hpp"
#include "webserv_utils.hpp"

extern char **environ;

namespace
{
	inline std::vector<const char *> ConvertToCstrVector(const std::vector<std::string> &strings)
	{
		std::vector<const char *> ptrs;
		for (std::vector<std::string>::const_iterator it = strings.begin(); it != strings.end();
			 ++it) {
			ptrs.push_back(it->c_str());
		}
		ptrs.push_back(NULL);
		return ptrs;
	}
} // namespace

namespace cgi
{

	void CgiResponse::ChildProcess()
	{
		try {
			parent_fd_.Close();
			Result<void>              res  = utils::SetSignalHandler(SIGPIPE, SIG_IGN, 0);
			if (res.IsErr()) {
				std::cerr << res.Err() << std::endl;
				exit(1);
			}
			if (dup2(child_fd_.GetFd(), STDIN_FILENO) == -1 ||
				dup2(child_fd_.GetFd(), STDOUT_FILENO) == -1) {
				perror("dup2");
				exit(1);
			}
			std::vector<std::string>  args = CreateArgs();
			std::vector<std::string>  envs = CreateEnvs();
			std::vector<const char *> argv = ConvertToCstrVector(args);
			std::vector<const char *> envp = ConvertToCstrVector(envs);
			execve(
				location_conf_.GetCgiPath().Value().c_str(),
				const_cast<char *const *>(argv.data()),
				const_cast<char *const *>(envp.data())
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
