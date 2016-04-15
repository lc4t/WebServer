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



int Server::testServerHello()
{
	int serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);		// create socket
	struct sockaddr_in serverAddr;										// bind IP/port
	std::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;									// IPV4
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");				// bind IP
	serverAddr.sin_port = htons(8888);									// bind port
	bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

	listen(serverSocket, 20);

	// Receive from client
	struct sockaddr_in clientAddr;
	socklen_t clientAddrSize = sizeof(clientAddr);
	int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);

	// Send to client
	char str[] = "hello visitor";
	write(clientSocket, &str, sizeof(str));

	// Close socket
	close(clientSocket);
	close(serverSocket);

	return 0;
}


int Server::testServerEcho()
{
	int serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);		// create socket
	struct sockaddr_in serverAddr;										// bind IP/port
	std::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;									// IPV4
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");				// bind IP
	serverAddr.sin_port = htons(8888);									// bind port
	bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

	listen(serverSocket, 20);


	// Receive from client
	while(true)
	{
		struct sockaddr_in clientAddr;
		socklen_t clientAddrSize = sizeof(clientAddr);
		int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);

		// Read from client
		char receiveStr[8192] = {0};
		while(strcmp(receiveStr, "exit") != 0)
		{
			read(clientSocket, &receiveStr, sizeof(receiveStr));
			std::cout << receiveStr << std::endl;
			write(clientSocket, &receiveStr, sizeof(receiveStr));
			std::memset(&receiveStr, 0, sizeof(receiveStr));
		}
		close(clientSocket);
	}



	// Close socket

	close(serverSocket);

	return 0;
}



int Server::testServerFileDownload(char* filename)
{
	int serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);		// create socket
	struct sockaddr_in serverAddr;										// bind IP/port
	std::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;									// IPV4
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");				// bind IP
	serverAddr.sin_port = htons(8888);									// bind port
	bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

	listen(serverSocket, 20);


	// File read
	FILE *fp = fopen(filename, "rb");
	if (fp == NULL)
	{
		std::cout << "Cannot open file " << filename << std::endl;
		return -1;
	}

	// Receive from client
	struct sockaddr_in clientAddr;
	socklen_t clientAddrSize = sizeof(clientAddr);
	int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);

	// Send to client
	char buffer[8192] = {0};
	int nCount = 0;
	while( (nCount = fread(buffer, 1, sizeof(buffer), fp)) > 0)
	{
		write(clientSocket, &buffer, nCount);	// must nCount, not sizeof it
	}


	// Close socket
	close(clientSocket);
	close(serverSocket);

	return 0;
}



Server::~Server()
{

}

