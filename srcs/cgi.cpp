/*
HeaderSection header;

Values val = header.at("content-length");

HeaderSectionからメタ変数を構築する
forkで子プロセスとしてcgiスクリプトを起動後、子プロセスの中でputenvして環境変数を設定

標準ストリームをプロセス間でパイプする

pipe -> fork -> putenv -> execve(args) -> std::cout << cgi-response -> read(STDOUT);

httpメッセージボディがあれば、content-lengthメタ変数を設定する必要がある

*/
#include "cgi.hpp"
#include "webserv_utils.hpp"

#include <cerrno>
#include <cstring>
#include <sys/types.h>
#include <unistd.h>

Cgi::Cgi(const http::RequestMessage &message) : message_(message)
{
	if (pipe(pipe_to_cgi) < 0) {
		throw std::runtime_error("pipe: " + std::string(strerror(errno)));
	}
}

/*
MUSTなメタ変数
https://wiki.suikawiki.org/n/CGI%E3%83%A1%E3%82%BF%E5%A4%89%E6%95%B0#section-%E3%83%A1%E3%82%BF%E5%A4%89%E6%95%B0%E3%81%AE%E4%B8%80%E8%A6%A7
*/
void Cgi::SetMetaVariables()
{
	if (message_.HasMessageBody()) {
		SetContentLength();
	}
	// MUSTなメタ変数を設定していく
}

//メタ変数の構築の仕方はそれぞれ異なるかもしれないので、Set関数を作って最後にまとめてputenvする
void Cgi::SetContentLength()
{
	meta_variables_.push_back(message_.field_lines_.GetKeyValueString("content-length"));
}

ssize_t Cgi::WriteRequestData(size_t nbyte) const
{
	return write(pipe_to_cgi[WRITE], message_.message_body_.c_str(), nbyte);
}

//環境変数としてPut
//スクリプト命令行の設定
int Cgi::StartCgiProcess()
{
	int pipe_from_cgi[2];

	if (pipe(pipe_from_cgi) < 0) {
		std::runtime_error("pipe: " + std::string(strerror(errno)));
	}

	pid_t pid = fork();

	if (pid == -1) {
		throw std::runtime_error("fork: " + std::string(strerror(errno)));
	}
	if (pid == 0) {
		close(pipe_to_cgi[WRITE]);
		close(pipe_from_cgi[READ]);
		dup2(pipe_to_cgi[READ], STDIN_FILENO);
		close(pipe_to_cgi[READ]);
		dup2(pipe_from_cgi[WRITE], STDOUT_FILENO);
		close(pipe_from_cgi[WRITE]);

		// httpリクエストのqueryから、cgiの引数を作成

		// stringのvectorをchar*のvectorに変換
		std::vector<char *> envp;

		for (std::vector<std::string>::iterator it = meta_variables_.begin();
			 it != meta_variables_.end();
			 ++it) {
			envp.push_back(const_cast<char *>(it->c_str()));
		}

		// execve(path, query, envp);
	} else {
		close(pipe_to_cgi[READ]);
		close(pipe_from_cgi[WRITE]);
	}
}
