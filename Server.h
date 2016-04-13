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
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

class Server
{
	public:
		Server();
		///TEST FUNCTION
		int testServer();

		///
		virtual ~Server();

	private:
		int httpd;
};

#endif /* SERVER_H_ */
