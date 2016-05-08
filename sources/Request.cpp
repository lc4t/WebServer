#include "../headers/Request.h"

Request::Request()
{
    this->requestlength = 0;
    this->countRN = 0;
}


bool Request::add(std::string str)
{
    this->countRN = 0;
    std::cout << "--:" << str << std::endl;
    // if (this->countRN == 1)
    // {
    //     this->requestlength += str.length();
    // }

    // #ifdef DEBUG
    //     std::cout << "29#RN:" << this->requestlength << std::endl;
    // #endif
    if (str == "\r\n")
    {
        this->countRN++;
        std::cout << "30#: \\r\\n over#"<< this->countRN << std::endl;
        return true;
    }
    if (this->isSetHeaders(HEAD_LINE))
    {

        // if (this->countRN == 1) // waiting for params
        // {
        //     this->params += str;
        //     #ifdef DEBUG
        //         std::cout << "50#postParamsis:" << this->params << std::endl;
        //     #endif
        //     return true;
        // }
        // this->countRN = 0;
        std::string key;
        std::string value;

        int length = str.length();   // key:value
        for (int i = 0; i < length; i++)
        {
            if (str[i] == ':' && str[i + 1] == ' ')
            {
                key = str.substr(0, i);
                value = str.substr(i + 2, length - i - 2 - 2);   // -2 because \r\n
                #ifdef DEBUG
                    std::cout << "31Add Headers#:<" << key << ">" << "<" << value << ">" << std::endl;
                #endif
                this->headers[key] = value;
                return true;
            }
        }

    }
    else
    {
        // this->countRN = 0;
        if (this->setRequestLine(str))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

int Request::getCountRN()
{
    return this->countRN;
}

bool Request::isEnd()
{
    #ifdef DEBUG
        std::cout << "38isEnd#:<" << this->getMethod() << ">" << "<" << this->getHeaderByName("Content-Length") << ">" << std::endl;
    #endif
    if (this->getMethod() == "GET")
    {
        return (this->countRN == 1);
    }
    else if (this->getMethod() == "POST" && this->countRN == 1)
    {
        std::cout << "32#isendPOST:" << this->isSetHeaders("Content-Length") << ":"<< this->requestlength << ":" <<  atoi(this->getHeaderByName("Content-Length").data()) << std::endl;
        if (this->isSetHeaders("Content-Length") && this->params.length() == this->getContentLength())
        {
            #ifdef DEBUG
                std::cout << "33isEnd#:<" << "True" << std::endl;
            #endif
            return true;
        }
    }
    return false;
}

bool Request::setRequestLine(std::string requestLine)
{
    #ifdef DEBUG
        std::cout << "39#:" <<  requestLine << std::endl;
    #endif
    int length = requestLine.length();
    if (length < 14 || length > 2048)
    {
        #ifdef DEBUG
            std::cout << "33#:" << "Not request line." << std::endl;
        #endif
        return false;
    }
    std::string split[3];
    // METHOD PATH VERSION
    for (int leftPos = 0, rightPos = 0, count = 0; rightPos < length && count < 3; rightPos++)
    // count is make to split, left right pos is to find a slipt
    {
        if (requestLine[rightPos] == ' ' || requestLine[rightPos] == '\r')
        {
            split[count++] = requestLine.substr(leftPos, rightPos - leftPos);
            leftPos = rightPos + 1;
        }
    }
    this->method = split[0];
    this->path = this->setPath(split[1]);
    this->params = this->setParams(split[1]);
    this->version = split[2];
    this->headers[HEAD_LINE] = requestLine;
    return true;

}

bool Request::isSetHeaders(std::string key)
{
    #ifdef DEBUG
        std::cout << "38isset#:" << key << ":" << !(this->headers.find(key) == this->headers.end()) << std::endl;
    #endif
    if (!(this->headers.find(key) == this->headers.end())) // true is not found
    {
        return true;
    }
    else
    {
        return false;
    }
}

std::pair<std::string, std::string> Request::setPath(std::string str)
{
    #ifdef DEBUG
        std::cout << "34#path:" << str << std::endl;
    #endif
    std::pair<std::string, std::string> path;
    int length = str.length();
    int questionMarkPos = INIT_POS;//
    int virgulePos = INIT_POS;//= str.length() - 1 - 2;      // /
    for (int i = length - 1; i >= 0; i--)
    {
        if (str[i] == '/')
        {
            virgulePos = i;
            break;  // find last /
        }   // logic: if / first find, no ?
        else if (str[i] == '?')
        {
            questionMarkPos = i;
        }
        else
        {

        }

    }
    if (virgulePos != INIT_POS)
    {
        path.first = str.substr(0, virgulePos + 1);
        if (questionMarkPos != INIT_POS && questionMarkPos - virgulePos > 1)
        {
            path.second = str.substr(virgulePos + 1 , questionMarkPos - virgulePos - 1);
        }
        else if (questionMarkPos == INIT_POS && length - virgulePos > 1)
        {
            path.second = str.substr(virgulePos + 1 , questionMarkPos - virgulePos - 1);
        }
        else
        {
            // path.second = DEFAULT_HTML_PAGE;
            path.second = EMPTY_STRING;
        }
    }
    else
    {
        path.first = EMPTY_STRING;
        path.second = EMPTY_STRING;
    }

    return path;
}

std::string Request::setParams(std::string str)
{
    std::map<std::string, std::string> params;
    int length = str.length();
    int leftPos = -1;
    for (int i = 0; i <= length - 2; i++)
    {
        if (str[i] == '?')
        {
            leftPos = i;
            break;
        }
    }
    if (leftPos == INIT_POS)
    {
        return EMPTY_STRING;
    }
    std::string paramsStr = str.substr(leftPos + 1, length - 2 - leftPos + 1);
    std::cout << paramsStr << std::endl;
    return EMPTY_STRING;
}

std::string Request::getPostLine()
{
    // std::string postLine = "Content-Type";
    return "URLencode";
}

int Request::getContentLength()
{
    return atoi(this->getHeaderByName("Content-Length").data());
}

int Request::setPostParams(std::string str)
{
    this->params = str;
}

std::string Request::getMethod()
{
    if (this->isSetHeaders(HEAD_LINE))
    {
        return this->method;
    }
    else
    {
        return EMPTY_STRING;
    }
}

std::string Request::getDir()
{
    if (this->isSetHeaders(HEAD_LINE))
    {
        return this->path.first;
    }
    else
    {
        //  error
        return EMPTY_STRING;
    }
}


std::string Request::getFilename()
{
    if (this->isSetHeaders(HEAD_LINE))
    {
        return this->path.second;
    }
    else
    {
        //  error
        return EMPTY_STRING;
    }
}


std::string Request::getParams()
{
    return this->params;
}


std::string Request::getProtocol()
{
    if (this->version != EMPTY_STRING)
    {
        return this->version;
    }
    else
    {
        //  error
        return EMPTY_STRING;
    }
}

std::string Request::getHeaderByName(std::string name)
{
    return this->headers[name];
}
