/* WebServer */
#include <iostream>
#include "headers/Server.h"

int main(int argc, const char** argv)
{
	std::cout << "start" << std::endl;
	Server* server = new Server();
//	server->testServerFileDownload("/home/lc4t/Documents/git/WebServer/example.conf");
//	server->testServerEcho();
	// server->testMultiThread();
    // server->start();
    server->testResponse();
	return 0;
}
