#include "cgi_request.hpp"
#include "webserv_utils.hpp"

using cgi::RequestMessage;

RequestMessage::RequestMessage(const http::RequestMessage &message)
	: message_(message), formdata_(message.request_line_.request_target_.GetRequestFormData())
{
	SearchScriptPath();
}

const http::RequestMessage RequestMessage::GetHttpRequest() const
{
	return message_;
}

const std::string RequestMessage::GetScriptPath() const
{
	return script_path_;
}

const std::vector<std::string> RequestMessage::GetMetaVariables() const
{
	return meta_variables_;
}

void RequestMessage::SearchScriptPath()
{
	std::vector<ThinString> hierarchy = Split(formdata_.path_, "/");

	for (std::vector<ThinString>::iterator it = hierarchy.begin(); it != hierarchy.end(); it++) {
		std::string lower_path = it->ToString();
		script_path_ += lower_path;
		if (!IsRegularFile(script_path_) || !EndsWith(lower_path, ".php")) {
			continue;
		}
		for (++it; it != hierarchy.end(); it++) {
			extra_path_ += it->ToString();
		}
	}
}

// MUSTなメタ変数
// https://wiki.suikawiki.org/n/CGI%E3%83%A1%E3%82%BF%E5%A4%89%E6%95%B0#section-%E3%83%A1%E3%82%BF%E5%A4%89%E6%95%B0%E3%81%AE%E4%B8%80%E8%A6%A7
void RequestMessage::SetMetaVariables(
	const std::string server_name, const std::string server_port, const std::string client_ip
)
{
	SetGateWayInterFace();
	SetQueryString();
	SetRequestMethod();
	SetServerProtocol();
	SetServerSoftWare();
	SetServerName(server_name);
	SetServerPort(server_port);
	SetRemoteAddr(client_ip);

	if (message_.HasMessageBody()) {
		SetContentLength();
	}
	if (message_.field_lines_.Contains("content-type")) {
		SetContentType();
	}
	SetScriptName();
	SetPathInfo();
}

// FIX transfer-encofingヘッダでチャンク化の場合は、復号して本体の長さを決定しなければならない
void RequestMessage::SetContentLength()
{
	meta_variables_.push_back(message_.field_lines_.GetKeyValueString("content-length"));
}

void RequestMessage::SetContentType()
{
	meta_variables_.push_back(message_.field_lines_.GetKeyValueString("content-type"));
}

void RequestMessage::SetGateWayInterFace()
{
	meta_variables_.push_back("gateway-interface=CGI/1.1");
}

void RequestMessage::SetPathInfo()
{
	meta_variables_.push_back(MakeKeyValueString("path-info", extra_path_));
}

void RequestMessage::SetScriptName()
{
	meta_variables_.push_back(MakeKeyValueString("script-name", script_path_));
}

void RequestMessage::SetQueryString()
{
	meta_variables_.push_back(MakeKeyValueString("query-string", formdata_.query_.ToString()));
}

void RequestMessage::SetRemoteAddr(const std::string &remote_addr)
{
	meta_variables_.push_back(MakeKeyValueString("remote-addr", remote_addr));
}

void RequestMessage::SetRequestMethod()
{
	meta_variables_.push_back(
		MakeKeyValueString("request-method", message_.request_line_.method_.ToString())
	);
}

void RequestMessage::SetServerName(const std::string &server_name)
{
	meta_variables_.push_back(MakeKeyValueString("server-name", server_name));
}

void RequestMessage::SetServerPort(const std::string &server_port)
{
	meta_variables_.push_back(MakeKeyValueString("server-port", server_port));
}

void RequestMessage::SetServerProtocol()
{
	meta_variables_.push_back(
		MakeKeyValueString("server-protocol", message_.request_line_.http_version_.ToString())
	);
}

void RequestMessage::SetServerSoftWare()
{
	meta_variables_.push_back("server-software=webserv/1.0");
}

std::string RequestMessage::MakeKeyValueString(const std::string &key, const std::string &value)
{
	return key + "=" + value;
}
