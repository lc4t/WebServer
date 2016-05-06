#ifndef MANAGE_H_
#define MANAGE_H_

#include "../config.hpp"

#include "Request.h"
#include "StaticPage.h"
#include "PHP.h"

class Manage
{
    public:
        Manage(int clientSocket, std::string clientIP, int clientPort);
        int start();

    private:
        int clientSocket;
        std::string clientIP;
        int clientPort;

        //handle

        int end();
};

#endif
