/*
 * Server.cpp
 *
 *  Created on: 2016年4月12日
 *      Author: lc4t
 */

#include "Server.h"

Server::Server()
{
	// TODO Auto-generated constructor stub

}



int Server::testServer()
{
	int serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);		// create socket
	struct sockaddr_in serverAddr;										// bind ip/port
	std::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;										// IPV4
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");				// bind ip
	serverAddr.sin_port = htons(8888);									// bind port
	bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

	listen(serverSocket, 20);

	//recive from client
	struct sockaddr_in clientAddr;
	socklen_t clientAddrSize = sizeof(clientAddr);
	int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);

	//send to client
	char str[] = "hello visitor";
	write(clientSocket, str, sizeof(str));

	//close socket
	close(clientSocket);
	close(serverSocket);

	return 0;
}

Server::~Server()
{
	// TODO Auto-generated destructor stub
}

