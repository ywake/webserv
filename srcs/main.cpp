#include <cstdlib>

#include "exception/config_exceptions.hpp"
#include "listen_exception.hpp"
#include "server.hpp"
#include "server_confs.hpp"

Result<conf::ServerConfs> OpenConfig(std::string filepath);

int main(int argc, char **argv)
{
	if (argc != 3) {
		std::cerr << "Usage: ./webserv <config_file>" << std::endl;
		return EXIT_FAILURE;
	}
	try {
		Result<conf::ServerConfs> open_conf = OpenConfig(argv[2]);
		if (open_conf.IsErr()) {
			std::cerr << open_conf.ErrMsg() << std::endl;
			return EXIT_FAILURE;
		}
		Result<void> res = Run(open_conf.Val());
		if (res.IsErr()) {
			std::cerr << res.Err() << std::endl;
			return EXIT_FAILURE;
		}
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
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
		server::Server server;
		try {
			server                  = server::Server(config);
			Result<void> listen_res = server.Listen();
			if (listen_res.IsErr()) {
				return listen_res.Err();
			}
		} catch (const ListenException &e) {
			return Error(e.what());
		}
		try {
			server.Run();
		} catch (const std::exception &e) {
			continue;
		}
	}
	return Result<void>();
}
