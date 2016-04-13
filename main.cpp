/* WebServer */



#include <iostream>
#include "Server.h"


int main(int argc, const char** argv)
{
	std::cout << "start" << std::endl;
	Server* server = new Server();
	server->testServer();
	return 0;
}
