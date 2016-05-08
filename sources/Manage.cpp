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

    struct timeval timeout = {CLIENT_TIMEOUT, 0};
    setsockopt(this->clientSocket, SOL_SOCKET,SO_RCVTIMEO, (char*)&timeout,sizeof(struct timeval));
    // setsockopt(this->clientSocket, SOL_SOCKET,SO_SNDTIMEO, (char*)&timeout,sizeof(struct timeval));
    socklen_t len=sizeof(timeout);
    while(true)
    {
        std::memset(&receiveStr, 0, sizeof(receiveStr));
        // receive timeout
        if (errno == EWOULDBLOCK)
        {
            #ifdef DEBUG
                std::cout << "8#:time out" << std::endl;
            #endif
            break;
        }

        // read by char, send to Request
        char c = '\0';
        int i = 0;
        int t = 0;
        while((t = read(this->clientSocket, &c, sizeof(c))) > 0 && request->getCountRN() < 1)
        {
            #ifdef DEBUG
                std::cout << "61#t=" << t << receiveStr << std::endl;
            #endif
            receiveStr[i++] = c;
            if (c == '\n')
            {
                #ifdef DEBUG
                    std::cout << "62#Add: " << receiveStr << std::endl;
                #endif
                if (!request->add(receiveStr))
                {
                    #ifdef DEBUG
                        std::cout << "9#:Cannot add headers: " << receiveStr << std::endl;
                    #endif
                }
                break;
            }
        }

        while(request->getMethod() == "POST" && request->getCountRN() == 1)
        {
            std::memset(&receiveStr, 0, sizeof(receiveStr));
            if (request->getPostLine() == "URLencode")
            {

                int length = request->getContentLength();
                for (int i = 0; i < length; i++)
                {
                    read(this->clientSocket, &c, sizeof(c));
                    receiveStr[i] = c;
                }
                request->setPostParams(receiveStr);
                std::cout << "63#" << receiveStr << "#" << request->getParams() << std::endl;
                break;
            }
        }



        // get post content


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
        std::cout << "-1#:CLOSE:" << this->clientIP << ":" << this->clientPort << std::endl;
    #endif
    shutdown(this->clientSocket,  SHUT_RDWR);
    close(this->clientSocket);

    return SUCCESS;
}
