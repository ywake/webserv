#ifndef LISTENER_HPP
#define LISTENER_HPP

// TODO fdclass
class Listener
{
  private:
	int fd_;

  public:
	Listener(int fd);
	int Accept();
};

#endif
