#ifndef EMPTIABLE_HPP
#define EMPTIABLE_HPP

template <typename T>
class Emptiable
{
  private:
	T    value_;
	bool is_empty_;

  public:
	Emptiable() : value_(), is_empty_(true){};
	Emptiable(const T &v) : value_(v), is_empty_(false){};
	Emptiable(const Emptiable &e) : value_(e.value_), is_empty_(e.is_empty_){};
	~Emptiable(){};

	bool empty() const
	{
		return is_empty_;
	}

	const T &Value() const
	{
		return value_;
	}

	T &Value()
	{
		return value_;
	}

	const Emptiable<T> &operator=(const Emptiable<T> &rhs)
	{
		if (this != &rhs) {
			value_    = rhs.value_;
			is_empty_ = rhs.is_empty_;
		}
		return *this;
	}

	const Emptiable<T> &operator=(const T &v)
	{
		value_    = v;
		is_empty_ = false;
		return *this;
	}

	bool operator==(const Emptiable<T> &rhs) const
	{
		return value_ == rhs.value_ && is_empty_ == rhs.is_empty_;
	}

	bool operator!=(const Emptiable<T> &rhs) const
	{
		return !(*this == rhs);
	}
};

#endif
