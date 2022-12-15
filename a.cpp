#include <iostream>
#include <set>

class A
{
  private:
	const std::string &s;
	int                a;

  private:
	A &operator=(const A &a);
	A(const A &);

  public:
	A(const std::string &s2) : s(s2) {}
	const std::string &S()
	{
		std::cerr << &s << std::endl;
		return s;
	}
	void setA(int i)
	{
		a = i;
	}
	int AA()
	{
		return a;
	}
};

int main()
{
	// std::set<A *> st;

	// st.insert(new A("aaa"));
	// st.insert(new A("bbb"));
	// (*st.begin())->setA(2);
	// for (std::set<A *>::iterator itr = st.begin(); itr != st.end();) {
	// 	A                      *a  = *itr;
	// 	std::set<A *>::iterator it = st.erase(itr);
	// 	itr                        = it;
	// 	delete a;
	// }
	// std::cout << st.size() << std::endl;
	std::set<std::string *> st;
	for (std::set<std::string *>::iterator it = st.begin(); it != st.end();) {
		std::set<std::string *>::iterator itr = st.erase(it);
	}
}
