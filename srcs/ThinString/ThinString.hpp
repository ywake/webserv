#ifndef THINSTRING_HPP
#define THINSTRING_HPP
#include <map>
#include <string>

/*
	The string class to avoid deepcopy
*/

class ThinString
{
  private:
	typedef std::map<const std::string, std::size_t> ReferenceCount;

  private:
	static ReferenceCount reference_count_;
	const std::string *content_;
	std::size_t start_;
	std::size_t end_;

  private:
	ThinString();

  public:
	ThinString(const ThinString &other);
	ThinString(const std::string &str, std::size_t start = 0, std::size_t end = -1);
	ThinString(const char *str, std::size_t start = 0, std::size_t end = -1);
	~ThinString();

  private:
	void init(const std::string &str);

  public:
	std::size_t len() const;
	std::string ToString() const;
	// std::pair<ThinString, ThinString> DivideBy(const std::string &delim) const;
	// std::size_t MeasureUntil(const std::string &delim) const;

  public:
	ThinString &operator=(const ThinString &rhs);
	// ThinString &operator+=(const ThinString &rhs);
	// ThinString operator+(const ThinString &rhs);
};

#endif
