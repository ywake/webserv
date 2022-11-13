#ifndef CGI_HPP
#define CGI_HPP

#include "buffer.hpp"
#include "cgi_request.hpp"
#include "http_response_builder.hpp"
#include "i_resource.hpp"
#include "request_message.hpp"
#include "response_message.hpp"
#include "result/result.hpp"

class Cgi : public http::IResource
{
  private:
	enum PIPE_TYPE {
		READ,
		WRITE,
		TYPE_SIZE
	};

	enum State {
		kParseHeader,
		kParseBody,
		kParseFinish,
	};

	State                    state_;
	buffer::Buffer           read_buffer_;
	buffer::MessageBuffer    msg_buffer_;
	cgi::HttpResponseBuilder builder_;

	cgi::RequestMessage cgi_request_;
	int                 pipe_to_cgi_[TYPE_SIZE];
	pid_t               pid_;

  public:
	Cgi(const http::RequestMessage &message);
	Result<void> Run(const std::string &cgi_path);
	bool         Terminate() const;
	void         SetRequestData(
				const std::string server_name, const std::string server_port, const std::string client_ip
			);
	ssize_t                        WriteRequestData(size_t nbyte) const;
	const Result<PollInstructions> Send();
	const Result<PollInstructions> Receive();

  private:
	Result<int>               StartCgiProcess(const char *file, char **argv, char **envp) const;
	Result<std::vector<char>> Read(size_t nbyte) const;
	const Result<void>        ParseCgiResponse();
	std::string               GetLine();
};

#endif // CGI_HPP
