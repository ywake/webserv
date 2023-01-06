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
	void
	CgiResponse::ExecChild(ManagedFd &child_fd, const StringArray &args, const StringArray &envs)
	{
		try {
			log(COL_RED "child process" COL_END);
			child_fd.CloseOther();
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
			const std::string &script_path = args.Strings()[1];
			Result<void>       cd_res      = ChangeDir(utils::GetDirName(script_path));
			if (cd_res.IsErr()) {
				log("cgi", cd_res.Err());
				exit(1);
			}
			execve(
				args.CArray()[0],
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

} // namespace cgi
