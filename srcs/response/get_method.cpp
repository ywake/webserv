#include <cerrno>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "autoindex.hpp"
#include "debug.hpp"
#include "get_method.hpp"
#include "http_define.hpp"
#include "http_exceptions.hpp"
#include "mime_type.hpp"
#include "stat.hpp"
#include "webserv_utils.hpp"

namespace response
{
	GetMethod::GetMethod(const server::IRequest &request, const conf::LocationConf &location)
		: AResponse(request), location_(location), managed_fd_()
	{
		log("GET", request.Path());
		const FullPath &root = location_.GetRoot();
		TryValidateRequestPath(root, request_.Path());
		const PartialPath resolved = ResolveIndexFilePath(request_.Path());
		if (utils::EndWith(resolved, "/")) {
			if (location_.GetAutoindex().empty() || !location_.GetAutoindex().Value()) {
				throw http::ForbiddenException();
			}
			ExecuteAutoIndex(root, resolved);
		} else {
			FullPath abs_path = utils::JoinPath(root, resolved);
			Stat     st       = TryStat(abs_path);
			if (st.IsDirectory()) {
				ExecuteDirectoryRedirect(resolved + "/");
			} else if (st.IsRegularFile()) {
				PrepareSendFile(abs_path, st.FileSize());
			} else {
				throw http::NotFoundException();
			}
		}
		// TODO index-files, headeres
	}

	void GetMethod::TryValidateRequestPath(const FullPath &root, const PartialPath &request_path)
	{
		TryStat(utils::JoinPath(root, request_path));
	}

	GetMethod::PartialPath GetMethod::ResolveIndexFilePath(const PartialPath &request_path) const
	{
		PartialPath path = request_path;

		log("found index file", path);
		while (utils::EndWith(path, "/")) {
			Result<PartialPath> index_file_path = FindReadableIndexFile(path);
			if (index_file_path.IsErr()) {
				break;
			}
			path = index_file_path.Val();
			log("found index file", path);
		}
		return path;
	}

	Result<GetMethod::PartialPath> GetMethod::FindReadableIndexFile(const PartialPath &base_path
	) const
	{
		typedef conf::LocationConf::IndexFiles IndexFiles;

		const IndexFiles &index_files = location_.GetIndexFiles();
		for (IndexFiles::const_iterator it = index_files.begin(); it != index_files.end(); ++it) {
			const PartialPath index_path = utils::JoinPath(base_path, *it);
			const FullPath    abs_path   = utils::JoinPath(location_.GetRoot(), index_path);
			if (Stat::FromPath(abs_path).IsErr() || !utils::IsReadablePath(abs_path)) {
				continue;
			}
			Result<PartialPath> normalized = utils::NormalizePath(index_path);
			if (normalized.IsErr()) {
				continue;
			}
			return normalized.Val();
		}
		return Error();
	}

	void GetMethod::ExecuteDirectoryRedirect(const PartialPath &request_path)
	{
		log("no slash dir redirect", request_path);
		std::string page = utils::CreateDefaultPage(http::StatusCode::kMovedPermanently);
		MetaDataStorage::StoreStatusLine(http::kHttpVersion, http::StatusCode::kMovedPermanently);
		MetaDataStorage::StoreHeader("Server", http::kServerName);
		MetaDataStorage::StoreHeader("Content-Type", "text/html");
		MetaDataStorage::StoreHeader("Content-Length", utils::ToString(page.size()));
		MetaDataStorage::StoreHeader("Connection", request_.NeedToClose() ? "close" : "keep-alive");
		MetaDataStorage::StoreHeader("Location", CreateLocationUrl(request_path));
		MetaDataStorage::PushWithCrLf();
		push_back(page);
		is_finished_ = true;
	}

	std::string GetMethod::CreateLocationUrl(const PartialPath &path) const
	{
		return "http://" + utils::JoinPath(request_.Authority(), path);
	}

	void GetMethod::PrepareSendFile(const FullPath &path, std::size_t file_size)
	{
		managed_fd_ = TryOpen(path);
		MetaDataStorage::StoreStatusLine(http::kHttpVersion, http::StatusCode::kOK);
		MetaDataStorage::StoreHeader("Server", http::kServerName);
		MetaDataStorage::StoreHeader("Content-Type", http::MimeType::GetMimeType(path));
		MetaDataStorage::StoreHeader("Content-Length", utils::ToString(file_size));
		MetaDataStorage::StoreHeader("Connection", request_.NeedToClose() ? "close" : "keep-alive");
		MetaDataStorage::PushWithCrLf();
	}

	void GetMethod::ExecuteAutoIndex(const FullPath &root, const PartialPath &request_path)
	{
		log("autoindex", request_path);
		std::string autoindex = TryCreateAutoIndex(root, request_path);
		MetaDataStorage::StoreStatusLine(http::kHttpVersion, http::StatusCode::kOK);
		MetaDataStorage::StoreHeader("Server", http::kServerName);
		MetaDataStorage::StoreHeader("Content-Type", "text/html");
		MetaDataStorage::StoreHeader("Content-Length", utils::ToString(autoindex.size()));
		MetaDataStorage::StoreHeader("Connection", request_.NeedToClose() ? "close" : "keep-alive");
		MetaDataStorage::PushWithCrLf();
		push_back(autoindex);
		is_finished_ = true;
	}

	std::string
	GetMethod::TryCreateAutoIndex(const FullPath &root, const PartialPath &request_path) const
	{
		result::Result<std::string> autoindex = autoindex::CreateAutoIndex(root, request_path);
		if (autoindex.IsErr()) {
			log("autoindx", autoindex.Err());
			if (autoindex.Err() == err::opendir::kEAccess) {
				throw http::ForbiddenException();
			} else if (autoindex.Err() == err::opendir::kENoEnt || autoindex.Err() == err::opendir::kENotDir) {
				throw http::NotFoundException();
			} else {
				throw http::InternalServerErrorException();
			}
		}
		return autoindex.Val();
	}

	Stat GetMethod::TryStat(const std::string &path) const
	{
		result::Result<Stat> st = Stat::FromPath(path);
		if (st.IsErr()) {
			result::ErrCode err = st.Err();
			if (err == Stat::kEAcces || err == Stat::kELoop) {
				throw http::ForbiddenException();
			} else if (err == Stat::kENotDir || err == Stat::kNoEnt || err == Stat::kENameTooLong) {
				throw http::NotFoundException();
			} else {
				throw http::InternalServerErrorException();
			}
		}
		return st.Val();
	}

	int GetMethod::TryOpen(const std::string &filename) const
	{
		int fd = open(filename.c_str(), O_RDONLY | O_NONBLOCK | O_CLOEXEC);
		if (fd == -1) {
			switch (errno) {
			case EACCES:
			case ENXIO:
			case ELOOP:
				throw http::ForbiddenException();
			case ENOTDIR:
			case ENOENT:
			case ENAMETOOLONG:
				throw http::NotFoundException();
			default:
				throw http::InternalServerErrorException();
			}
		}
		return fd;
	}

	void GetMethod::Perform(const event::Event &event)
	{
		(void)event;
		ssize_t read_size = Read(managed_fd_.GetFd());
		if (read_size < 0) {
			throw http::InternalServerErrorException();
		}
		if (read_size == 0) {
			is_finished_ = true;
		}
	}

	bool GetMethod::HasFd() const
	{
		return managed_fd_.GetFd() != ManagedFd::kNofd;
	}

	Emptiable<int> GetMethod::GetFd() const
	{
		return managed_fd_.GetFd();
	}

} // namespace response
