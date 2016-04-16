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
#include <iostream>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

class Server
{
	public:
		Server();
		///TEST FUNCTION
		int testServerHello();
		int testServerEcho();
		int testServerFileDownload(char* filename);
		int testMultiThread();
		///
		virtual ~Server();

	private:
		typedef struct clientArgs
		{
			int clientSocket;
			std::string clientIP;
			int clientPort;

		}clientArgs;
		static void* testMultiThreadFunction(void* testMultiThreadArgs);
};

#endif /* SERVER_H_ */
