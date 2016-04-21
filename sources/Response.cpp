/*
 * Response.cpp
 *
 *  Created on: 2016年4月19日
 *      Author: lc4t
 */

#include "../headers/Response.h"

Response::Response()
{
    request = new Request();
    time_t t =  time(0);
    char timeStr[64];
    strftime(timeStr, sizeof(timeStr), "%a, %d %b %Y %X %Z", localtime(&t));
    this->Date = timeStr;

    this->responseStr = new char[8192];
    std::memset(this->responseStr, 0, sizeof(this->responseStr));
    this->status = -1;
    this->page = new FILE;
}

// Response::Response(Request request)
// {
//     this->request = request;
// }

Request* Response::getRequest()
{
    return this->request;
}


char* Response::getResponseHeaders()
{
    if (this->request->isEnd())
    {
        this->status = 200;
        // return this->status200();
    }
    if(!this->setPage())
    {
        std::cout << "error in Response::setPage()" << std::endl;
        this->status = 500;
        this->setPage();
    };

    switch(this->status)
    {
        // RFC 2616
        case 200:
            return this->status200();
        case 404:
            return this->status404();
        case 500:
            return this->status500();
        default:
            return NULL;
    }
}

char* Response::status200()
{

    sprintf(this->responseStr, "%s%s", this->responseStr, "HTTP/1.1 200 OK\r\n");
    sprintf(this->responseStr, "%s%s", this->responseStr, this->Date.data());
    sprintf(this->responseStr, "%s%s", this->responseStr, "\r\n");
    sprintf(this->responseStr, "%s%s", this->responseStr, "Content-Type: text/html;charset=ISO-8859-1\r\n");
    // add more
    sprintf(this->responseStr, "%s%s", this->responseStr, "\r\n");
    return this->responseStr;
}



char* Response::status403()
{
    sprintf(this->responseStr, "%s%s", this->responseStr, "HTTP/1.1 403 Forbidden\r\n");
    sprintf(this->responseStr, "%s%s", this->responseStr, this->Date.data());
    sprintf(this->responseStr, "%s%s", this->responseStr, "\r\n");
    sprintf(this->responseStr, "%s%s", this->responseStr, "Content-Type: text/html;charset=ISO-8859-1\r\n");
    // add more
    sprintf(this->responseStr, "%s%s", this->responseStr, "\r\n");
    return this->responseStr;
}


char* Response::status404()
{
    
    sprintf(this->responseStr, "%s%s", this->responseStr, "HTTP/1.1 404 Not Found\r\n");
    sprintf(this->responseStr, "%s%s", this->responseStr, this->Date.data());
    sprintf(this->responseStr, "%s%s", this->responseStr, "\r\n");
    sprintf(this->responseStr, "%s%s", this->responseStr, "Content-Type: text/html;charset=ISO-8859-1\r\n");
    // add more
    sprintf(this->responseStr, "%s%s", this->responseStr, "\r\n");
    return this->responseStr;
}

char* Response::status500()
{
    
    sprintf(this->responseStr, "%s%s", this->responseStr, "HTTP/1.1 500 Internal Server Error\r\n");
    sprintf(this->responseStr, "%s%s", this->responseStr, this->Date.data());
    sprintf(this->responseStr, "%s%s", this->responseStr, "\r\n");
    sprintf(this->responseStr, "%s%s", this->responseStr, "Content-Type: text/html;charset=ISO-8859-1\r\n");
    // add more
    sprintf(this->responseStr, "%s%s", this->responseStr, "\r\n");
    return this->responseStr;
}

bool Response::setPage()
{
    char* filename = new char[2048];

    std::cout << "getPath: " << this->request->getPath() << std::endl;
    std::cout << "getFile: " << this->request->getFile() << std::endl;

    if (this->request->getPath().length() == 0 && this->request->getFile().data() == "/")
    {
        std::cout << "list dir-->, but not safe" << std::endl;
        this->status = 403;
    }

    switch(this->status)
    {
        case 200:
            std::memset(filename, 0, 2048);
            sprintf(filename, "%s%s", filename, "/home/lc4t/Documents/git/WebServer/html");
            sprintf(filename, "%s%s", filename, this->request->getPath().data());
            sprintf(filename, "%s%s", filename, this->request->getFile().data());
            std::cout << "200:" << filename << std::endl;

            this->page = fopen(filename, "rb");
            if (this->page == NULL)
            {
                // 404
                std::cout << "Cannot open file " << filename << std::endl;
                this->status = 404;
                std::cout << "404:" << filename << std::endl;
                this->page = fopen("/home/lc4t/Documents/git/WebServer/html/404.html", "rb");
            }
            else
            {
                this->status = 200;
            }
            return true;
        case 500:
            std::memset(filename, 0, 2048);
            sprintf(filename, "%s%s", filename, "/home/lc4t/Documents/git/WebServer/html");
            sprintf(filename, "%s%s", filename, "/");
            sprintf(filename, "%s%s", filename, "500.html");
            std::cout << "500:" << filename << std::endl;
            this->page = fopen(filename, "rb");
            assert(this->page != NULL);
            
            std::cout << "Cannot open file " << filename << std::endl;
            return true;
        default:
            return false;
            
    }
    


}


FILE* Response::getPage()
{

    return this->page;
}



