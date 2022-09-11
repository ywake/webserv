#ifndef THIN_STRING_HPP
#define THIN_STRING_HPP
#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>

/*
	The string class to avoid deepcopy
*/

class ThinString
{
  private:
	typedef std::set<std::string>					   StringSet;
	typedef std::map<const std::string *, std::size_t> ReferenceCount;

  public:
	typedef std::string::const_iterator		  const_iterator;
	typedef std::pair<ThinString, ThinString> ThinStrPair;
	typedef enum {
		kAlignLeft		= 0 << 0,
		kAlignRight		= 1 << 0,
		kKeepDelimLeft	= 1 << 1,
		kKeepDelimRight = 1 << 2
	} DelimFlag;

  private:
	static ReferenceCount reference_count_;
	static StringSet	  base_set_;
	const std::string	  *base_;
	std::size_t			  start_;
	std::size_t			  length_;

  public:
	ThinString();
	ThinString(const ThinString &other, std::size_t start = 0, std::size_t length = ~0UL);
	ThinString(const std::string &str, std::size_t start = 0, std::size_t length = ~0UL);
	ThinString(const char *str, std::size_t start = 0, std::size_t length = ~0UL);
	~ThinString();

  private:
	ThinString(const std::string *base, std::size_t start, std::size_t length);
	void init(const std::string &str);
	ThinString
	CreateLeftSide(const std::string &delim, std::size_t size, unsigned int delim_side) const;
	ThinString
	CreateRightSide(const std::string &delim, std::size_t start, unsigned int delim_side) const;

  public:
	bool		empty() const;
	std::size_t len() const;
	std::size_t size() const;
	const char &at(size_t n) const;
	const char &back() const;

	std::size_t find(const std::string &str, std::size_t pos = 0) const;
	std::size_t find(const char *s, std::size_t pos = 0) const;
	std::size_t find(char ch, std::size_t pos = 0) const;

	ThinString	substr(std::size_t pos = 0, std::size_t size = ~0UL) const;
	std::string ToString() const;
	ThinStrPair DivideBy(const std::string &delim, unsigned int delim_flag = kAlignLeft) const;
	std::size_t MeasureUntil(const std::string &delim) const;

	const_iterator begin() const;
	const_iterator end() const;

	ThinString &operator=(const ThinString &rhs);
	// ThinString &operator+=(const ThinString &rhs);
	// ThinString operator+(const ThinString &rhs);

	bool operator==(const ThinString &rhs) const;
	bool operator==(const std::string &rhs) const;
	bool operator==(const char *rhs) const;
	bool operator!=(const ThinString &rhs) const;
	bool operator!=(const std::string &rhs) const;
	bool operator!=(const char *rhs) const;
};

std::ostream &operator<<(std::ostream &os, const ThinString &thin_str);

#endif
