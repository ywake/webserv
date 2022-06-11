#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "fd.hpp"

class Socket : public Fd
{
  private:
	virtual Result< void > Create() = 0;

  protected:
	char *host_;
	char *port_;

  public:
	/*
	 * Constructor/Destructor
	 */
	Socket();
	Socket(char *host, char *port);
	Socket(Socket const &copy);
	~Socket();

	/*
	 * Operators
	 */
	Socket &operator=(Socket const &other);

	/*
	 * Getter/Setter
	 */

	/*
	 * Methods
	 */
};

#endif /* SOCKET_HPP */
