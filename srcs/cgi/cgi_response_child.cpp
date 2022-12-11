#include "cgi_response.hpp"

namespace cgi
{
	static Result<void> Dup2(int old_fd, int new_fd);

	void CgiResponse::ChildProcess()
	{
		std::vector<char *> args;
		std::vector<char *> envs;
		CreateArgs(args);
		CreateEnvs(envs);

		close(parent_fd_.GetFd());
		if (Dup2(child_fd_.GetFd(), STDIN_FILENO).IsErr()) {
			exit(1);
		}
		if (Dup2(child_fd_.GetFd(), STDOUT_FILENO).IsErr()) {
			exit(1);
		}
		if (execve(resource_path_.c_str(), args.data(), envs.data()) < 0) {
			exit(1);
		}
	}

	void CgiResponse::CreateArgs(std::vector<char *> args)
	{
		Emptiable<std::string> cgi_path = location_conf_.GetCgiPath();
		if (cgi_path.empty()) {
			// cgi_pathがemptyでない時コンストラクトされるので、ありえないはず
			throw http::InternalServerErrorException();
		}
		args.push_back(const_cast<char *>(cgi_path.Value().c_str()));
		args.push_back(const_cast<char *>(resource_path_.c_str()));
		args.push_back(NULL);
	}

	void CgiResponse::CreateEnvs(std::vector<char *> envs)
	{
		for (size_t i = 0; environ[i] != NULL; ++i) {
			envs.push_back(environ[i]);
		}
		envs.push_back(NULL);
	}

	static Result<void> Dup2(int old_fd, int new_fd)
	{
		if (old_fd == new_fd) {
			return Result<void>();
		}
		if (dup2(old_fd, new_fd) != new_fd) {
			return Error("dup2 error");
		}
	}
} // namespace cgi
