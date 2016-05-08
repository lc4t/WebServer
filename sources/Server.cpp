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
    serverAddr.sin_addr.s_addr = inet_addr(LOCALHOST_IP);                // bind IP
    serverAddr.sin_port = htons(SERVER_PORT);                                    // bind port
    bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    listen(serverSocket, SERVER_CLIENT_NUM);
    #ifdef DEBUG
        std::cout << "1#:Listen @ " << SERVER_PORT << std::endl;
    #endif
    while(true)
    {
        #ifdef DEBUG
    	   std::cout << "2#:Waiting new connection" << std::endl;
        #endif
        struct sockaddr_in clientAddr;
        socklen_t clientAddrSize = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);
        if (clientSocket == ERROR_EXIT)
        {
            #ifdef DEBUG
                std::cout << "3#:Accept Error." << std::endl;
            #endif
            continue;
        }
        else
        {
            #ifdef DEBUG
                std::cout << "4#:ACCEPT: " << inet_ntoa(clientAddr.sin_addr) << ":" << clientAddr.sin_port << std::endl;
            #endif
        }
        clientArgs* clientArgsVoid = new clientArgs;
        clientArgsVoid -> clientSocket = clientSocket;
        clientArgsVoid -> clientIP = inet_ntoa(clientAddr.sin_addr);
        clientArgsVoid -> clientPort = clientAddr.sin_port;
        // Read from client
        pthread_t thread;
        if (pthread_create(&thread, NULL, this->clientHandle, (void*)clientArgsVoid) == ERROR_EXIT)
        {
            #ifdef DEBUG
                std::cout << "5#:Error when create a new thread." << std::endl;
            #endif
            break;
        }
        // void** status;
        pthread_join(thread, NULL);
    }

    int shutdownThreadRet = shutdown(serverSocket,SHUT_WR);
    if (shutdownThreadRet == ERROR_EXIT)
    {
        #ifdef DEBUG
            std::cout << "6#:shutdownThread Error." << std::endl;
        #endif
    }

    // Close socket
    close(serverSocket);

    return 0;
}

void* Server::clientHandle(void* clientArgsVoid)
{
    clientArgs* clientSocketArgs = reinterpret_cast<clientArgs*>(clientArgsVoid);
    Manage* manager = new Manage(clientSocketArgs->clientSocket,  clientSocketArgs->clientIP, clientSocketArgs->clientPort);

    if (manager->start() != SUCCESS)
    {
        #ifdef DEBUG
            std::cout << "7#:Not exit success." << std::endl;
        #endif
    }



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
    shutdown(clientSocket,  SHUT_RDWR);
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
        shutdown(clientSocket,  SHUT_RDWR);
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
    shutdown(clientSocket,  SHUT_RDWR);
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
    shutdown(clientSocketArgs -> clientSocket,  SHUT_RDWR);
    close(clientSocketArgs -> clientSocket);
}

int Server::testResponse()
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


        char responseStr[8192] = {0};


        sprintf(responseStr, "%s%s", responseStr, "HTTP/1.1 200 OK\r\n");
        sprintf(responseStr, "%s%s", responseStr, "Date: Sat, 31 Dec 2005 23:59:59 GMT\r\n");
        sprintf(responseStr, "%s%s", responseStr, "Content-Type: text/html;charset=ISO-8859-1\r\n");


        sprintf(responseStr, "%s%s", responseStr, "\r\n");
        std::cout << write(clientSocket, &responseStr, strlen(responseStr)) << std::endl;
        std::memset(&responseStr, 0, sizeof(responseStr));

        FILE *fp = fopen("/home/lc4t/Documents/git/WebServer/html/200.html", "rb");
        if (fp == NULL)
        {
            std::cout << "Cannot open file " << "/home/lc4t/Documents/git/WebServer/html/200.html" << std::endl;
            return -1;
        }
        char buffer[8192];
        int nCount = 0;
        while( (nCount = fread(buffer, 1, sizeof(buffer), fp)) > 0)
        {
            std::cout << write(clientSocket, &buffer, nCount) << std::endl;    // must nCount, not sizeof it
        }
        std::memset(&buffer, 0, sizeof(buffer));

        shutdown(clientSocket,  SHUT_RDWR);
        close(clientSocket);
    }



    // Close socket

    close(serverSocket);

    return 0;
}

Server::~Server()
{

}
