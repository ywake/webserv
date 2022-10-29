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
	//メタ変数の設定
	SetMetaVariables();
	// cgiスクリプトの起動
	StartCgiProcess();
}

//要求データの設定 httpメッセージボディを書き出す
//環境変数としてPut
//スクリプト命令行の設定
int Cgi::StartCgiProcess()
{
	static const int READ  = 0;
	static const int WRITE = 1;
	int              pipe_to_cgi[2];
	int              pipe_from_cgi[2];

	if (pipe(pipe_to_cgi) < 0 || pipe(pipe_from_cgi) < 0) {
		throw std::runtime_error("pipe: " + std::string(strerror(errno)));
	}

	pid_t pid;

	pid = fork();

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
		// std::cout << message_.message_body_ << std::endl;
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
	meta_variables_.push_back(RebuildHeader("content-length"));
}

// HeaderSectionに持たせるメンバ関数かもしれない。keyとvalueを結合して"key=value"の文字列にする。ToStringという名前にしよう。
std::string Cgi::RebuildHeader(const std::string &name) const
{
	std::string meta_variable_value = "";
	std::string meta_variable_name  = utils::ToLowerString(name);

	HeaderSection::Values values = message_.field_lines_.at(meta_variable_name);
	for (HeaderSection::Values::iterator it = values.begin(); it != values.end(); it++) {
		meta_variable_value += it->GetValue();
	}

	std::string meta_variable = meta_variable_name + "=" + meta_variable_value;
	return meta_variable;
}
