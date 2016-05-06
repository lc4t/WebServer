#include "../headers/Request.h"

Request::Request()
{
    this->requestlength = 0;
    this->doubleRN = false;
}


bool Request::add(std::string str)
{
    this->requestlength += str.length();
    #ifdef DEBUG
        std::cout << "29#CurrentLength:" << this->requestlength << std::endl;
    #endif
    if (str == "\r\n")
    {
        std::cout << "30#: \\r\\n over" << std::endl;
        this->doubleRN = true;
        return true;
    }
    if (this->isSetHeaders(HEAD_LINE))
    {
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

bool Request::isEnd()
{
    if (this->getMethod() == "GET")
    {
        return this->doubleRN;
    }
    else if (this->getMethod() == "POST" && this->isSetHeaders("Content_Length"))
    {
        std::cout << "32#isendPOST:" << this->requestlength << ":" <<  atoi(this->getHeaderByName("Content_Length").data()) << std::endl;
        if (this->requestlength >= atoi(this->getHeaderByName("Content_Length").data()))
        {
            return true;
        }
    }
    return false;
}

bool Request::setRequestLine(std::string requestLine)
{
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
    if (this->headers.find(key) == this->headers.end())
    {
        return false;
    }
    else
    {
        return true;
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
