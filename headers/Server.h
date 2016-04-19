/*
 * Server.h
 *
 *  Created on: 2016年4月12日
 *      Author: lc4t
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <arpa/inet.h>
#include <cstring>
// #include "Http.h"
#include <iostream>
#include <netinet/in.h>
#include <pthread.h>
#include "Response.h"
#include <signal.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER_PORT 8888
#define SERVER_CLIENT_NUM 3

class Server
{
    public:
        Server();
        // TEST FUNCTION
        int testServerHello();
        int testServerEcho();
        int testServerFileDownload(char* filename);
        int testMultiThread();
        int testResponse();
        ///

        int start();
        static void* clientHandle(void* clientArgsVoid);
        virtual ~Server();

    private:
        typedef struct clientArgs
        {
            int clientSocket;
            std::string clientIP;
            int clientPort;
        }clientArgs;
        // TEST FUNCTION
        static void* testMultiThreadFunction(void* testMultiThreadArgs);
        //

        FILE* httpAnalyse(std::string receiveStr);
};

#endif /* SERVER_H_ */
