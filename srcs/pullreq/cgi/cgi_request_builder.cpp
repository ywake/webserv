#include "cgi_request_builder.hpp"
#include "webserv_utils.hpp"

using cgi::CgiRequestBuilder;

CgiRequestBuilder::CgiRequestBuilder(const http::RequestMessage &message)
	: message_(message), formdata_(message.request_line_.request_target_.GetRequestFormData())
{
	SearchScriptPath();
}

const http::RequestMessage CgiRequestBuilder::GetHttpRequest() const
{
	return message_;
}

const std::string CgiRequestBuilder::GetScriptPath() const
{
	return script_path_;
}

const std::vector<std::string> CgiRequestBuilder::GetMetaVariables() const
{
	return meta_variables_;
}

void CgiRequestBuilder::SearchScriptPath()
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
void CgiRequestBuilder::SetMetaVariables(
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
void CgiRequestBuilder::SetContentLength()
{
	meta_variables_.push_back(message_.field_lines_.GetKeyValueString("content-length"));
}

void CgiRequestBuilder::SetContentType()
{
	meta_variables_.push_back(message_.field_lines_.GetKeyValueString("content-type"));
}

void CgiRequestBuilder::SetGateWayInterFace()
{
	meta_variables_.push_back("gateway-interface=CGI/1.1");
}

void CgiRequestBuilder::SetPathInfo()
{
	meta_variables_.push_back(MakeKeyValueString("path-info", extra_path_));
}

void CgiRequestBuilder::SetScriptName()
{
	meta_variables_.push_back(MakeKeyValueString("script-name", script_path_));
}

void CgiRequestBuilder::SetQueryString()
{
	meta_variables_.push_back(MakeKeyValueString("query-string", formdata_.query_.ToString()));
}

void CgiRequestBuilder::SetRemoteAddr(const std::string &remote_addr)
{
	meta_variables_.push_back(MakeKeyValueString("remote-addr", remote_addr));
}

void CgiRequestBuilder::SetRequestMethod()
{
	meta_variables_.push_back(
		MakeKeyValueString("request-method", message_.request_line_.method_.ToString())
	);
}

void CgiRequestBuilder::SetServerName(const std::string &server_name)
{
	meta_variables_.push_back(MakeKeyValueString("server-name", server_name));
}

void CgiRequestBuilder::SetServerPort(const std::string &server_port)
{
	meta_variables_.push_back(MakeKeyValueString("server-port", server_port));
}

void CgiRequestBuilder::SetServerProtocol()
{
	meta_variables_.push_back(
		MakeKeyValueString("server-protocol", message_.request_line_.http_version_.ToString())
	);
}

void CgiRequestBuilder::SetServerSoftWare()
{
	meta_variables_.push_back("server-software=webserv/1.0");
}

std::string CgiRequestBuilder::MakeKeyValueString(const std::string &key, const std::string &value)
{
	return key + "=" + value;
}
