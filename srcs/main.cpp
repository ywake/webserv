#include <csignal>
#include <cstdlib>
#include <iostream>

#include "debug.hpp"
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
		Result<conf::ServerConfs> config = OpenConfig(argv[1]);
		if (config.IsErr()) {
			std::cerr << config.ErrMsg() << std::endl;
			exit(EXIT_FAILURE);
		}
		Result<void> run_result = Run(config.Val());
		if (run_result.IsErr()) {
			std::cerr << run_result.Err() << std::endl;
			exit(EXIT_FAILURE);
		}
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
}

Result<conf::ServerConfs> OpenConfig(std::string filepath)
{
	try {
		return conf::ServerConfs::FromFilePath(filepath);
	} catch (const conf::ConfigException &e) {
		return Error(e.what());
	}
}

Result<void> Run(const conf::ServerConfs &config)
{
	while (true) {
		try {
			server::Server server(config);
			Result<void>   listen_res = server.Listen();
			if (listen_res.IsErr()) {
				return listen_res.Err();
			}
			server.Run();
		} catch (const ListenException &e) {
			return Error(e.what());
		} catch (const std::logic_error &e) {
			log("logic error exception");
			return Error(e.what());
		} catch (const std::exception &e) {
			std::cerr << e.what() << std::endl;
			std::cerr << "server auto restart" << std::endl;
			continue;
		}
	}
	return Result<void>();
}
