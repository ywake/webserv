#include <cerrno>
#include <cstdio>

#include "cgi_response.hpp"
#include "debug.hpp"
#include "meta_env.hpp"
#include "webserv_utils.hpp"

namespace
{
	Result<void> ChangeDir(const std::string &path)
	{
		if (chdir(path.c_str()) == -1) {
			return Error("chdir: " + std::string(strerror(errno)));
		}
		return Result<void>();
	}
} // namespace

namespace cgi
{
	void CgiResponse::ExecChild(
		const std::string &script_path, ManagedFd &child_fd, const StringArray &envs
	)
	{
		try {
			log("child process");
			parent_fd_.Close();
			Result<void> res = utils::SetSignalHandler(SIGPIPE, SIG_IGN, 0);
			if (res.IsErr()) {
				log("cgi", res.Err());
				exit(1);
			}
			if (dup2(child_fd.GetFd(), STDIN_FILENO) == -1 ||
				dup2(child_fd.GetFd(), STDOUT_FILENO) == -1) {
				perror("dup2");
				exit(1);
			}
			const std::string &cgi_path = location_conf_.GetCgiPath().Value();
			StringArray        argv     = CreateArgs(cgi_path, script_path, request_.Query());
			Result<void>       cd_res   = ChangeDir(utils::GetDirName(script_path));
			if (cd_res.IsErr()) {
				log("cgi", cd_res.Err());
				exit(1);
			}
			execve(
				cgi_path.c_str(),
				const_cast<char *const *>(argv.CArray()),
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
		const std::string &cgi_path, const std::string &script_path, const std::string &query
	)
	{
		std::vector<std::string> args;

		args.push_back(cgi_path);
		args.push_back(script_path);
		(void)query; // TODO query
		return args;
	}

} // namespace cgi
