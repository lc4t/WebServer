#include "../headers/Manage.h"


Manage::Manage(int clientSocket, std::string clientIP, int clientPort)
{
    this->clientSocket = clientSocket;
    this->clientIP = clientIP;
    this->clientPort = clientPort;
}

int Manage::start()
{
    char receiveStr[MAX_TRANSPORT_STR_LENGTH] = {0};
    Request* request = new Request();
    while(true)
    {
        std::memset(&receiveStr, 0, sizeof(receiveStr));
        // receive timeout
        struct timeval timeout = {CLIENT_TIMEOUT, 0};
        if (setsockopt(this->clientSocket, SOL_SOCKET,SO_RCVTIMEO, (char*)&timeout,sizeof(struct timeval)) == -1)
        {
            #ifdef DEBUG
                std::cout << "8#:time out" << std::endl;
            #endif
            break;
        }

        // read by char, send to Request
        char c = '\0';
        int i = 0;
        while(read(this->clientSocket, &c, sizeof(c)) > 0)
        {
            receiveStr[i++] = c;
            if (c == '\n')
            {
                // std::cout << "Add: " << receiveStr << std::endl;
                if (!request->add(receiveStr))
                {
                    #ifdef DEBUG
                        std::cout << "9#:Cannot add headers: " << receiveStr << std::endl;
                    #endif
                }
                break;
            }
        }

        // check and response
        if (request->isEnd())
        {
            #ifdef DEBUG
                std::cout << "11#" << request->getMethod() << request->getDir() << request->getFilename() << request->getParams() << std::endl;
                std::cout << "12#send response --> " << std::endl;
            #endif

            // get path, split to filetype<.xxx, MIME>
            std::pair<std::string, std::string> filetype;
            std::string filename = request->getFilename();
            for (int i = filename.length() -  1; i >= 0; i--)
            {
                if (filename[i] == '.')
                {
                    filetype.first = filename.substr(i);
                    break;
                }
            }

            //  .xxx and MIME, then give the next class
            int type = NOT_SET_PAGE;
            if (filetype.first == ".html" || filetype.first == ".png" ||filetype.first == ".txt" || filetype.first == ".jpg" || filetype.first == ".ico")
            {
                if (filetype.first == ".html")
                {
                    filetype.second = "text/html";
                }
                else if (filetype.first == ".png")
                {
                    filetype.second = "image/png";
                }
                else if (filetype.first == ".txt")
                {
                    filetype.second = "text/plain";
                }
                else if (filetype.first == ".jpg")
                {
                    filetype.second = "image/jpeg";
                }
                else if (filetype.first == ".ico")
                {
                    filetype.second = "image/x-icon";
                }
                else
                {
                    // exception
                }

                type = STATIC_PAGE_TYPE;
            }

            // PHP
            else if (filetype.first == ".php")
            {
                if (filetype.first == ".php")
                {
                    filetype.second = "text/html";
                }
                else
                {
                    // exception
                }
                type = PHP_PAGE_TYPE;
            }

            // empty filename
            else if (filetype.first == EMPTY_STRING)
            {
                filetype.second = "text/html";
                type = STATIC_PAGE_TYPE;
            }

            // not set this file
            else
            {
                filetype.second = "text/plain";
                type = STATIC_PAGE_TYPE;
            }


            #ifdef DEBUG
                std::cout << "14#filetype:" << filetype.first << filetype.second << "@" << type << std::endl;
            #endif

            switch(type)
            {
                case STATIC_PAGE_TYPE:
                {
                    // send headers
                    StaticPage* page = new StaticPage(request->getProtocol(), request->getDir(), request->getFilename(), filetype.second);
                    char* responseHeadersStr = new char[MAX_TRANSPORT_STR_LENGTH]();
                    responseHeadersStr = page->getResponseHeader();
                    std::cout << "15#resHeaders:" << write(this->clientSocket, responseHeadersStr, strlen(responseHeadersStr)) << std::endl;
                    delete responseHeadersStr;

                    FILE* file = new FILE;
                    file = page->getPage();
                    // char buffer[MAX_TRANSPORT_STR_LENGTH];
                    char* buffer = new char[MAX_TRANSPORT_STR_LENGTH]();
                    int nCount = 0;
                    while( (nCount = fread(buffer, 1, sizeof(buffer), file)) > 0)
                    {
                        std::cout << "16#page:" << write(this->clientSocket, buffer, nCount) << std::endl;    // must nCount, not sizeof it
                    }
                    delete buffer;
                    delete file;

                    break;
                }

                case PHP_PAGE_TYPE:
                {
                    PHP* page = new PHP(request->getProtocol(), request->getDir(), request->getFilename(), filetype.second);
                    break;
                }

                case NOT_SET_PAGE:
                    break;
                default:
                    break;
            }


            break;
        }

    }

    if (this->end() == SUCCESS)
    {
        return SUCCESS;
    }

    return ERROR_EXIT;

}



int Manage::end()
{
    #ifdef DEBUG
        std::cout << "12#:CLOSE:" << this->clientIP << ":" << this->clientPort << std::endl;
    #endif
    shutdown(this->clientSocket,  SHUT_RDWR);
    close(this->clientSocket);

    return SUCCESS;
}
