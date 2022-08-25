#ifndef THINSTRING_HPP
#define THINSTRING_HPP
#include <algorithm>
#include <iostream>
#include <map>
#include <string>

/*
	The string class to avoid deepcopy
*/

class ThinString
{
  private:
	typedef std::map<const std::string, std::size_t> ReferenceCount;

  public:
	typedef std::string::const_iterator const_iterator;

  private:
	static ReferenceCount reference_count_;
	const std::string *base_;
	std::size_t start_;
	std::size_t length_;

  public:
	ThinString();
	ThinString(const ThinString &other, std::size_t start = 0, std::size_t length = -1);
	ThinString(const std::string &str, std::size_t start = 0, std::size_t length = -1);
	ThinString(const char *str, std::size_t start = 0, std::size_t length = -1);
	~ThinString();

  private:
	void init(const std::string &str);

  public:
	std::size_t len() const;
	std::size_t size() const;
	std::size_t find(const std::string &str, std::size_t pos = 0) const;
	std::size_t find(const char *s, std::size_t pos = 0) const;
	std::size_t find(char ch, std::size_t pos = 0) const;

	ThinString substr(std::size_t pos = 0, std::size_t size = -1) const;
	std::string ToString() const;
	// std::pair<ThinString, ThinString> DivideBy(const std::string &delim) const;
	// std::size_t MeasureUntil(const std::string &delim) const;
	const_iterator begin() const;
	const_iterator end() const;

	ThinString &operator=(const ThinString &rhs);
	// ThinString &operator+=(const ThinString &rhs);
	// ThinString operator+(const ThinString &rhs);
};

std::ostream &operator<<(std::ostream &os, const ThinString &thin_str);

#endif

/*

(1) size_type find( const basic_string& str, size_type pos = 0 ) const;
(2) size_type find( const CharT* s, size_type pos, size_type count ) const;
(3) size_type find( const CharT* s, size_type pos = 0 ) const;
(4) size_type find( CharT ch, size_type pos = 0 ) const;

指定された文字シーケンスと等しい最初の部分文字列を探します。 検索は pos から開始されます。
つまり見つかった文字列が pos より前の位置で始まることはありません。

1) str と等しい最初の部分文字列を探します。
2) 範囲 [s, s+count) と等しい最初の部分文字列を探します。 範囲はヌル文字を含んでいても構いません。
3) s の指す文字列と等しい最初の部分文字列を探します。 文字列の長さは Traits::length(s)
を用いて最初のヌル文字によって決定されます。
4) ch と等しい最初の文字を探します(後述する形式的なルールで単一文字の部分文字列として扱われます)。
5) std::basic_string_view<CharT,Traits> sv = t; によって行われたかのように、 t を文字列ビュー sv
に暗黙に変換し、 sv と等しい最初の部分文字列を探します。
このオーバーロードは、std::is_convertible_v<const T&, std::basic_string_view<CharT, Traits>> が true
であり、 std::is_convertible_v<const T&, const CharT*> が false
である場合にのみ、オーバーロード解決に参加します。 形式的には、部分文字列 str
は、以下のすべてが真である場合、位置 xpos で見つかったと言います。

xpos >= pos
xpos + str.size() <= size()
str 内のすべての位置 n について Traits::eq(at(xpos+n), str.at(n))
特に、これは以下のことを暗に示します。

部分文字列は pos <= size() - str.size() の場合にのみ見つかります。
空の部分文字列は pos <= size() の場合にのみ pos で見つかります。
空でない部分文字列に対して pos >= size() の場合この関数は必ず npos を返します。
*/
