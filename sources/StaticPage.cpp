#include "../headers/StaticPage.h"

StaticPage::StaticPage(std::string version, std::string dir, std::string filename, std::string contentType)
{
    // init
    this->dir = dir;
    this->filename = filename;
    this->responseHeadersStr = new char[MAX_TRANSPORT_STR_LENGTH]();
    this->page = new FILE;

    // will return FILE* finally
    this->ostreamType = FILE_POINTER;

    // guess this request is OK, set status 200
    this->status = 200;

    // Set Content-Type head
    this->headers["Content-Type"] = contentType;    // format: [Content-Type: MIME]

    // Set response head line head
    this->headers[HEAD_LINE] = version + " ";   // format: [version statusCode statusAlert]


}


char* StaticPage::getResponseHeader()
{
    this->setPage();
    #ifdef DEBUG
        std::cout << "54#getter:" << this->responseHeadersStr << std::endl;
    #endif
    this->setResponseHeader();
    #ifdef DEBUG
        std::cout << "55#getter:" << this->responseHeadersStr << std::endl;
    #endif
    return this->responseHeadersStr;
    //
}

int StaticPage::setPage()
{
    #ifdef DEBUG
        std::cout << "51#setpage:" << this->dir << "$" << this->filename << this->status << std::endl;
    #endif

    switch(this->status)
    {
        case 200:
        {
            if (this->dir == "/" && this->filename == "")   // visit root, no file
            {
                // go to the default page
                this->status = 200;

                char* path = new char[MAX_PATH_LENGTH]();

                std::sprintf(path, "%s%s", path, HTML_ROOT_PATH);
                std::sprintf(path, "%s%s", path, this->dir.data());
                std::sprintf(path, "%s%s", path, DEFAULT_HTML_PAGE);
                this->page = fopen(path, "rb");
                assert (this->page != NULL);


                // delete path;
                #ifdef DEBUG
                    std::cout << "55#:" << std::endl;
                #endif
                return SUCCESS;
            }
            // visit
            else
            {
                // secure , should set user
                // if (this->dir.find("../") != std::string::npos)
                // {
                //     std::cout << "52#danger:" << "What are you doing?" << std::endl;
                //     return ERROR_EXIT;
                // }

                // deal with request
                char* path = new char[MAX_PATH_LENGTH]();

                std::sprintf(path, "%s%s", path, HTML_ROOT_PATH);
                std::sprintf(path, "%s%s", path, this->dir.data());
                std::sprintf(path, "%s%s", path, this->filename.data());
                this->page = fopen(path, "rb");
                #ifdef DEBUG
                    std::cout << "52#path:" << path << "$" << HTML_ROOT_PATH << "$" << this->dir.data() << "$" << this->filename.data() << std::endl;
                #endif
                if(this->page == NULL)
                {
                    #ifdef DEBUG
                        std::cout << "57#:" << "NOT FOUND" << std::endl;
                    #endif
                    this->status = 404;
                    return this->setPage();
                }
                else
                {
                    return SUCCESS;
                }

            }
            break;
        }
        case 404:
        {
            this->status = 404;
            this->headers["Content-Type"] = "text/html";
            char* path = new char[MAX_PATH_LENGTH]();

            std::sprintf(path, "%s%s", path, HTML_ROOT_PATH);
            std::sprintf(path, "%s%s", path, NOT_FOUND_HTML);

            #ifdef DEBUG
                std::cout << "58#path:" << path << std::endl;
            #endif

            this->page = fopen(path, "rb");
            // delete path;
            assert (this->page != NULL);
            return SUCCESS;
            break;
        }
        case 500:
        {
            this->status = 500;
            char* path = new char[MAX_PATH_LENGTH]();

            std::sprintf(path, "%s%s", path, HTML_ROOT_PATH);
            std::sprintf(path, "%s%s", path, SEVER_ERROR_HTML);
            this->page = fopen(path, "rb");
            // delete path;
            assert (this->page != NULL);
            return SUCCESS;
            break;
        }
        default:
        {
            this->status = 500;
            return this->setPage();
        }
    }

}



int StaticPage::setResponseHeader()
{

    std::stringstream statusStream;
    std::string statusStr;
    statusStream << this->status;
    statusStream >> statusStr;
    this->headers[HEAD_LINE] += statusStr;
    this->headers[HEAD_LINE] += " ";
    switch(this->status)
    {
        case 200:
        {
            this->headers[HEAD_LINE] += "OK";
            break;
        }
        case 404:
        {
            this->headers[HEAD_LINE] += "Not Found";
            break;
        }
        case 500:
        {
            this->headers[HEAD_LINE] += "Internal Server Error";
            break;
        }
        default:
        {
            // exception
        }
    }
    #ifdef DEBUG
        std::cout << "56#:" << this->headers[HEAD_LINE] << std::endl;
        std::cout << "57#:" << this->responseHeadersStr << std::endl;
    #endif
    std::sprintf(this->responseHeadersStr, "%s%s", responseHeadersStr, this->headers[HEAD_LINE].data());
    std::sprintf(this->responseHeadersStr, "%s%s", responseHeadersStr, "\r\n");

    for (auto i : this->headers)
    {
        if (i.first == HEAD_LINE)
        {
            continue;
        }
        std::sprintf(this->responseHeadersStr, "%s%s", responseHeadersStr, i.first.data());
        std::sprintf(this->responseHeadersStr, "%s%s", responseHeadersStr, ": ");
        std::sprintf(this->responseHeadersStr, "%s%s", responseHeadersStr, i.second.data());
        std::sprintf(this->responseHeadersStr, "%s%s", responseHeadersStr, "\r\n");
    }
    std::sprintf(this->responseHeadersStr, "%s%s", responseHeadersStr, "\r\n");

    return SUCCESS;
}

FILE* StaticPage::getPage()
{
    return this->page;
}
