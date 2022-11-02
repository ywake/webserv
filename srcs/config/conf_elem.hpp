template <typename T>
class ConfElem
{
  private:
	T    value_;
	bool is_empty_;

  public:
	ConfElem() : value_(), is_empty_(true){};
	ConfElem(const T &v) : value_(v), is_empty_(false){};
	~ConfElem(){};

	bool empty() const
	{
		return is_empty_;
	}

	T &Value()
	{
		is_empty_ = false;
		return value_;
	}

	void ValueSet()
	{
		is_empty_ = false;
	}

	const ConfElem<T> &operator=(const ConfElem<T> &rhs)
	{
		value_    = rhs.value_;
		is_empty_ = rhs.is_empty_;
		return *this;
	}

	const ConfElem<T> &operator=(const T &v)
	{
		value_    = v;
		is_empty_ = false;
		return *this;
	}

	bool operator==(const ConfElem<T> &rhs) const
	{
		return value_ == rhs.value_;
	}
};
