/*
 * Server.cpp
 *
 *  Created on: 2016年4月12日
 *      Author: lc4t
 */

#include "../headers/Server.h"

Server::Server()
{
	signal(SIGPIPE,SIG_IGN);
}

int Server::start()
{
	int serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);        // create socket
    struct sockaddr_in serverAddr;                                        // bind IP/port
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;                                    // IPV4
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");                // bind IP
    serverAddr.sin_port = htons(SERVER_PORT);                                    // bind port
    bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    listen(serverSocket, SERVER_CLIENT_NUM);
    std::cout << "Listen @ " << SERVER_PORT << std::endl;
    // Receive from client
    // int count = 0;
    while(true)
    {
    	std::cout << "Waiting new connection" << std::endl;
        struct sockaddr_in clientAddr;
        socklen_t clientAddrSize = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);
        if (clientSocket == -1)
        {
            std::cout << "Accept Error." << std::endl;
            continue;
        }
        else
        {
            std::cout << "ACCEPT: " << inet_ntoa(clientAddr.sin_addr) << ":" << clientAddr.sin_port << std::endl;
        }
        clientArgs* clientArgsVoid = new clientArgs;
        clientArgsVoid -> clientSocket = clientSocket;
        clientArgsVoid -> clientIP = inet_ntoa(clientAddr.sin_addr);
        clientArgsVoid -> clientPort = clientAddr.sin_port;
        // Read from client
        pthread_t thread;
        if (pthread_create(&thread, NULL, this->clientHandle, (void*)clientArgsVoid) == -1)
        {
            std::cout << "Error when create a new thread.";
            break;
        }
        void** status;
        pthread_join(thread, status);
    }
    int shutdownThreadRet = shutdown(serverSocket,SHUT_WR);
    if (shutdownThreadRet == -1)
    {
        std::cout << "shutdownThread Error." << std::endl;
    }

    // Close socket

    close(serverSocket);

    return 0;
}

void* Server::clientHandle(void* clientArgsVoid)
{
	Http http;
	clientArgs* clientSocketArgs = reinterpret_cast<clientArgs*>(clientArgsVoid);
    char receiveStr[8192] = {0};
    while(true)
    {	
    	
    	// receive timeout
    	struct timeval timeout = {3, 0};
    	if (setsockopt(clientSocketArgs -> clientSocket, SOL_SOCKET,SO_RCVTIMEO, (char*)&timeout,sizeof(struct timeval)) == -1)
    	{
    		std::cout << "time out" << std::endl;
    		break;
    	}
    	
    	// read by char
    	char c = '\0';
    	int i = 0;
    	while(read(clientSocketArgs -> clientSocket, &c, sizeof(c)) > 0)
    	{
    		receiveStr[i++] = c;
    		if (c == '\n')
    		{
    			// std::cout << "(\\n)";
    			if (!http.addHeader(receiveStr))
    			{
					std::cout << "Cannot add headers: " << receiveStr << std::endl;
				}
				break;
    		}
    	}

    	std::memset(&receiveStr, 0, sizeof(receiveStr));
        if (http.isEnd())
        {
        	break;
        }        
    }
    // if (http.getStatus() == true)
    // {
    // 	std::cout << "get ,I will send your data." << std::endl;
    // }
    std::cout << "CLOSE:" << clientSocketArgs -> clientIP << ":" << clientSocketArgs -> clientPort << std::endl;
    close(clientSocketArgs -> clientSocket);
}

int Server::testServerHello()
{
    int serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);        // create socket
    struct sockaddr_in serverAddr;                                        // bind IP/port
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;                                    // IPV4
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");                // bind IP
    serverAddr.sin_port = htons(8888);                                    // bind port
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
    int serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);        // create socket
    struct sockaddr_in serverAddr;                                        // bind IP/port
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;                                    // IPV4
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");                // bind IP
    serverAddr.sin_port = htons(8888);                                    // bind port
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
    int serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);        // create socket
    struct sockaddr_in serverAddr;                                        // bind IP/port
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;                                    // IPV4
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");                // bind IP
    serverAddr.sin_port = htons(8888);                                    // bind port
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
        write(clientSocket, &buffer, nCount);    // must nCount, not sizeof it
    }


    // Close socket
    close(clientSocket);
    close(serverSocket);

    return 0;
}

int Server::testMultiThread()
{
    int serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);        // create socket
    struct sockaddr_in serverAddr;                                        // bind IP/port
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;                                    // IPV4
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");                // bind IP
    serverAddr.sin_port = htons(8888);                                    // bind port
    bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    listen(serverSocket, 2);
    std::cout << "Listen @ 8888" << std::endl;
    // Receive from client
    // int count = 0;
    while(true)
    {
    	std::cout << "Waiting new connection" << std::endl;
        struct sockaddr_in clientAddr;
        socklen_t clientAddrSize = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);
        if (clientSocket == -1)
        {
            std::cout << "Accept Error." << std::endl;
            continue;
        }
        else
        {
            std::cout << "ACCEPT: " << inet_ntoa(clientAddr.sin_addr) << ":" << clientAddr.sin_port << std::endl;
        }
        clientArgs* testMultiThreadArgs = new clientArgs;
        testMultiThreadArgs -> clientSocket = clientSocket;
        testMultiThreadArgs -> clientIP = inet_ntoa(clientAddr.sin_addr);
        testMultiThreadArgs -> clientPort = clientAddr.sin_port;
        // Read from client
        pthread_t thread;
        if (pthread_create(&thread, NULL, this->testMultiThreadFunction, (void*)testMultiThreadArgs) == -1)
        {
            std::cout << "Error when create a new thread.";
            break;
        }
        
    }
    int shutdownThreadRet = shutdown(serverSocket,SHUT_WR);
    if (shutdownThreadRet == -1)
    {
        std::cout << "shutdownThread Error." << std::endl;
    }

    // Close socket

    close(serverSocket);

    return 0;
}

void* Server::testMultiThreadFunction(void* testMultiThreadArgs)
{	
	clientArgs* clientSocketArgs = reinterpret_cast<clientArgs*>(testMultiThreadArgs);
    char receiveStr[8192] = {0};
    while(true)
    {
        read(clientSocketArgs -> clientSocket, &receiveStr, sizeof(receiveStr));
        std::cout << "RECEIVE: " << clientSocketArgs->clientIP << ":" << clientSocketArgs->clientPort << " " << receiveStr;
        if(strlen(receiveStr) == 0)
        {
        	break;
        }
        write(clientSocketArgs -> clientSocket, &receiveStr, sizeof(receiveStr));
        std::memset(&receiveStr, 0, sizeof(receiveStr));
    }
    std::cout << "CLOSE:" << clientSocketArgs->clientIP << ":" << clientSocketArgs->clientPort << std::endl;
    close(clientSocketArgs -> clientSocket);
}

Server::~Server()
{

}

