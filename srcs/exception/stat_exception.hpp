#ifndef STAT_EXCEPTION_HPP
#define STAT_EXCEPTION_HPP

#include <stdexcept>

class StatException : public std::exception
{
  private:
	int error_number_;

  public:
	StatException(int error_number);

	const char *what() const throw();

	enum StatError {
		kEAcces,
		kEBadFd,
		kEFault,
		kEIO,
		kELoop,
		kENameTooLong,
		kNoEnt,
		kENotDir,
		kEOverFlow,
		kENoMem,
		kEInval,
		kUnknown
	};

	StatError error() const;
};
#endif
