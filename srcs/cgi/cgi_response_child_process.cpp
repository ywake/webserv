#include "cgi_response.hpp"
#include "debug.hpp"
#include "meta_env.hpp"

extern char **environ;

namespace cgi
{
	static Result<void> Dup2(int old_fd, int new_fd);

	void CgiResponse::ChildProcess()
	{
		std::vector<const char *>          args = CreateArgs();
		Result<std::vector<const char *> > envs = CreateEnvs();
		if (envs.IsErr()) {
			exit(1);
		}
		parent_fd_.Close();
		if (Dup2(child_fd_.GetFd(), STDIN_FILENO).IsErr()) {
			exit(1);
		}
		if (Dup2(child_fd_.GetFd(), STDOUT_FILENO).IsErr()) {
			exit(1);
		}
		execve(
			location_conf_.GetCgiPath().Value().c_str(),
			const_cast<char *const *>(args.Val().data()),
			const_cast<char *const *>(envs.Val().data())
		);
		exit(1);
	}

	std::vector<const char *> CgiResponse::CreateArgs()
	{
		const Emptiable<std::string> &cgi_path = location_conf_.GetCgiPath();
		std::vector<const char *>     args;
		args.push_back(cgi_path.Value().c_str());
		args.push_back(resource_path_.c_str());
		args.push_back(NULL);
		return args;
	}

	Result<std::vector<const char *> > CgiResponse::CreateEnvs()
	{
		std::vector<const char *> envs;
		for (size_t i = 0; environ[i] != NULL; ++i) {
			envs.push_back(environ[i]);
		}
		SetMetaEnv(envs);
		envs.push_back(NULL);
		return envs;
	}

	void CgiResponse::SetMetaEnv(std::vector<const char *> &envs)
	{
		SetAuthType(envs, request_);
		SetContentLength(envs, request_);
		SetContentType(envs, request_);
	}

	static Result<void> Dup2(int old_fd, int new_fd)
	{
		if (old_fd == new_fd) {
			return Result<void>();
		}
		if (dup2(old_fd, new_fd) != new_fd) {
			return Error("dup2 error");
		}
		return Result<void>();
	}
} // namespace cgi
