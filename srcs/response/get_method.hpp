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
		void        ExecuteAutoIndex(const std::string &root, const std::string &request_path);
		void        ExecuteDirectoryRedirect(const std::string &request_path);
		std::string CreateLocationUrl(const std::string &path);
		void        PrepareSendFile(const std::string &path, std::size_t file_size);
		std::string TryCreateAutoIndex(const std::string &root, const std::string &request_path);
		int         TryOpen(const std::string &filename) const;
		Stat        TryStat(const std::string &path);
		std::string ResolveIndexFilePath(const std::string &request_path);
		Result<std::string> FindReadableIndexFile(const std::string &base_path);
		void TryValidateRequestPath(const std::string &root, const std::string &request_path);
	};
} // namespace response

#endif
