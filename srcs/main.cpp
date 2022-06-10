#include <map>
#include <vector>

int main(void)
{
	t_conf confs = Conf(filepath);

	Vector< Server > servers = init(confs); // socket, bind, listen

	run_event_loop(servers); // select, accept, recv, send
}
