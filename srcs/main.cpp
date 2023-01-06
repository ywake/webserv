#include <csignal>
#include <cstdlib>
#include <iostream>

#include "exception/config_exceptions.hpp"
#include "listen_exception.hpp"
#include "server.hpp"
#include "server_confs.hpp"
#include "webserv_utils.hpp"

Result<conf::ServerConfs> OpenConfig(std::string filepath);
Result<void>              Run(const conf::ServerConfs &config);

int main(int argc, char **argv)
{
	if (argc != 2) {
		std::cerr << "Usage: ./webserv <config_file>" << std::endl;
		exit(EXIT_FAILURE);
	}
	Result<void> res = utils::SetSignalHandler(SIGPIPE, SIG_IGN, 0);
	if (res.IsErr()) {
		std::cerr << res.Err() << std::endl;
		exit(EXIT_FAILURE);
	}
	try {
		Result<std::string> res = utils::ReadFile(argv[1]);
		if (res.IsErr()) {
			std::cerr << res.ErrMsg() << std::endl;
			exit(EXIT_FAILURE);
		}
		std::string       config_file_content = res.Val();
		conf::ServerConfs config(config_file_content);
		Result<void>      run_result = Run(config);
		if (run_result.IsErr()) {
			std::cerr << run_result.Err() << std::endl;
			exit(EXIT_FAILURE);
		}
	} catch (const conf::ConfigException &e) {
		std::cerr << e.what() << std::endl;
		exit(EXIT_FAILURE);
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
}

// Result<conf::ServerConfs> OpenConfig(std::string filepath)
// {
// 	try {
// 		return conf::ServerConfs::FromFilePath(filepath);
// 	} catch (const conf::ConfigException &e) {
// 		return Error(e.what());
// 	}
// }

Result<void> Run(const conf::ServerConfs &config)
{
	// while (true) {
	try {
		server::Server server(config);
		Result<void>   listen_res = server.Listen();
		if (listen_res.IsErr()) {
			return listen_res.Err();
		}
		server.Run();
	} catch (const ListenException &e) {
		return Error(e.what());
	}
	// try {
	// 	server.Run();
	// } catch (const std::exception &e) {
	// 	continue;
	// }
	// }
	return Result<void>();
}
