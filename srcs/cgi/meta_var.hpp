#include <string>

namespace cgi
{
	class MetaVar
	{
	  private:
		std::string name_;
		std::string value_;
		bool        is_case_sensitive_;

	  public:
		MetaVar();
		MetaVar(const MetaVar &other);
		MetaVar(const std::string &name, const std::string &value, bool is_case_sensitive);
		~MetaVar();

		const std::string &Name() const;
		const std::string &Value() const;
		bool               IsCaseSensitive() const;

		MetaVar &operator=(const MetaVar &rhs);
	};
} // namespace cgi
