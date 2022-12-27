#ifndef GET_METHOD_HPP
#define GET_METHOD_HPP

#include "a_response.hpp"
#include "location_conf.hpp"
#include "managed_fd.hpp"
#include "result.hpp"
#include "stat.hpp"

namespace response
{
	class GetMethod : public AResponse
	{
	  private:
		typedef std::string PartialPath;
		typedef std::string FullPath;

	  private:
		const conf::LocationConf &location_;
		ManagedFd                 managed_fd_;

	  private:
		GetMethod();
		GetMethod(const GetMethod &other);
		GetMethod &operator=(const GetMethod &rhs);

	  public:
		GetMethod(const server::IRequest &request, const conf::LocationConf &location);
		void           Perform(const event::Event &event);
		bool           HasFd() const;
		Emptiable<int> GetFd() const;

	  private:
		void        ExecuteAutoIndex(const FullPath &root, const PartialPath &request_path);
		void        ExecuteDirectoryRedirect(const PartialPath &request_path);
		std::string CreateLocationUrl(const PartialPath &path);
		void        PrepareSendFile(const FullPath &path, std::size_t file_size);
		std::string TryCreateAutoIndex(const FullPath &root, const PartialPath &request_path);
		int         TryOpen(const FullPath &filename) const;
		Stat        TryStat(const FullPath &path);
		PartialPath ResolveIndexFilePath(const PartialPath &request_path);
		Result<PartialPath> FindReadableIndexFile(const PartialPath &base_path);
		void TryValidateRequestPath(const FullPath &root, const PartialPath &request_path);
	};
} // namespace response

#endif
