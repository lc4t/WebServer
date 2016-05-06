#ifndef SERVER_H_
#define SERVER_H_


#include "../config.hpp"

#include "Manage.h"



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

        virtual ~Server();

    private:
        typedef struct clientArgs
        {
            int clientSocket;
            std::string clientIP;
            int clientPort;
        }clientArgs;
        // TEST FUNCTION
        static void* clientHandle(void* clientArgsVoid);
        static void* testMultiThreadFunction(void* testMultiThreadArgs);
        //

};

#endif
