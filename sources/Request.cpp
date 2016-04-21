/*
 * Request.cpp
 *
 *  Created on: 2016年4月16日
 *      Author: lc4t
 */

#include "../headers/Request.h"

Request::Request()
{
    this->requestlength = 0;
    this->doubleRN = false;
    this->status = true;
    
}

bool Request::Test()
{
    return this->doubleRN;
}

bool Request::addHeader(std::string str)
{

    // std::cout << "Header(" << str.length() << ") ";
    // std::cout << "(" << str << ")" << std::endl;
    this->requestlength += str.length();
    if (str == "\r\n")
    {
        std::cout << "receive: \\r\\n" << std::endl;
        this->doubleRN = true;
        // std::cout << "TEST:" << this->Test() << std::endl;
        return true;
    }
    if (this->isSetHeaders("requestLine"))
    {
        std::string key;
        std::string value;
        // std::cout << str << std::endl;
        int length = str.length();   // key:value
        for (int i = 0; i < length; i++)
        {
            if (str[i] == ':' && str[i + 1] == ' ')
            {
                key = str.substr(0, i);
                value = str.substr(i + 2, length - i - 2 - 2);   // -2 because \r\n
                
                std::cout << "<" << key << ">"<< std::endl;
                std::cout << "<" << value << ">" << std::endl;
                if (key == "Host")
                {
                    this->Host = value; 
                    this->headers.insert(key);

                    // std::cout << "set " << key <<": " << value << std::endl;

                }
                else if (key == "User-Agent" )
                {
                    this->User_Agent = value; 
                    this->headers.insert(key);

                    // std::cout << "set " << key << ": " << value << std::endl;

                }
                else if (key == "Accept")
                {
                    this->Accept = value; 
                    this->headers.insert(key);

                    // std::cout << "set " << key << ": " << value << std::endl;

                }
                else if (key == "Accept-Language" )
                {
                    this->Accept_Language = value; 
                    this->headers.insert(key);

                    // std::cout << "set " << key << ": " << value << std::endl;

                }
                else if (key == "Accept-Encoding")
                {
                    this->Accept_Encoding = value; 
                    this->headers.insert(key);

                    // std::cout << "set " << key << ": " << value << std::endl;

                }
                else if (key == "Keep-Alive")
                {
                    this->Keep_Alive = value; 
                    this->headers.insert(key);

                    // std::cout << "set " << key << ": " << value << std::endl;

                }
                else if (key == "Connection" )
                {
                    this->Connection = value; 
                    this->headers.insert(key);

                    // std::cout << "set " << key << ": " << value << std::endl;

                }
                else if (key == "Pragma" )
                {
                    this->Pragma = value; 
                    this->headers.insert(key);

                    // std::cout << "set " << key << ": " << value << std::endl;

                }
                else if (key == "Cache-Control")
                {
                    this->Cache_Control = value; 
                    this->headers.insert(key);

                    // std::cout << "set " << key << ": " << value << std::endl;

                }
                else if (key == "Upgrade-Insecure-Requests")
                {
                    // std::cout << "test here" << std::endl;
                    this->Upgrade_Insecure_Requests = value; 
                    this->headers.insert(key);
                    // std::cout << "test headers 2" << std::endl;
                    // std::cout << "set " << key << ": " << value << std::endl;

                }
                else if (key == "cookie")
                {
                    //
                    // this->Cookie['Cookie'] = formatCookie(value);
                    std::cout << "cookie " << value << std::endl;
                    this->headers.insert(key);

                }
                else
                {
                    std::cout << "Unknow headers." << std::endl;

                }
                return true;
            }
        }
    }
    else
    {
        // std::cout << "set request line here-->" << std::endl;
        if (this->setRequestLine(str))
        {
            // std::cout << "headers ok!" << std::endl;
            return true;
        }
        else
        {
            // std::cout << "cannot add headers." << std::endl;
            return false;
        }
    }
}




bool Request::isEnd()
{
    // std::cout << "END??" << this->doubleRN << "&" << this->status << std::endl;
    if (this->getMethod() == "GET")
    {
        return this->doubleRN && this->status;
    }

    return false;
}

bool Request::setRequestLine(std::string requestLine)
{
    std::cout << "<*" << requestLine << "*>" << std::endl;
    int length = requestLine.length();
    if (length < 14 || length > 2048)
    {
        std::cout << "Not request line." << std::endl;
        this->status = false;
        return false;
    }
    std::string split[3];
    // try
    // {
    for (int leftPos = 0, rightPos = 0, count = 0; rightPos < length && count < 3; rightPos++) 
    // count is make to split, left right pos is to find a slipt
    {
        if (requestLine[rightPos] == ' ' || requestLine[rightPos] == '\r')
        { 
            split[count++] = requestLine.substr(leftPos, rightPos - leftPos);
            leftPos = rightPos + 1;
            // std::cout << split[count - 1] << std::endl;
        }
    }
    this->method = split[0];
    if (this->method != "GET")
    {
        this->status = false;
        return false;
    }
    this->path = this->requestPathAnalyse(split[1]);
    // std::cout << "207: path.first:" << this->path.first << std::endl;
    this->params = this->requestParamsAnalyse(split[1]);  
    this->version = split[2];
    this->headers.insert("requestLine");
    // std::cout << "205: this>headers>requestLine" << this->headers["requestLine"] << std::endl;
    // std::cout << "Add headers request line done! " << std::endl;
    return true;
    // }
    // catch(std::exception& e)
    // {
    //     std::cout << "Standard exception: " << e.what() << std::endl;  
    //     this->status = false;
    //     return false;
    // }
    // return false;

    
}


bool Request::isSetHeaders(std::string key)
{
    // std::cout << "isset: " << key << " " << this->headers[key] << std::endl;
    if (this->headers.find(key) == this->headers.end())
    {
        // std::cout << "isset: " << key << " " << 0 << std::endl;
        return false;
    }
    else
    {
        // std::cout << "isset: " << key << " " << 1 << std::endl;
        return true;

    }
    // std::map<std::string, bool>::iterator iter;
    // iter = this->headers.find(key);
    // if (iter != this->headers.end())
    // {
    //     std::cout << "isset: " << key << " 1 " << this->headers[key] << std::endl;
    //     return true;
    // }
    // else
    // {
    //     std::cout << "isset: " << key << " 2 " << this->headers[key] << std::endl;
    //     return false;
    // }
}





std::pair<std::string, std::string> Request::requestPathAnalyse(std::string str)
{  
    std::pair<std::string, std::string> path;
    int length = str.length();
    int questionMarkPos = -1;// 
    int virgulePos = -1;//= str.length() - 1 - 2;      // /
    for (int i = length - 1; i >= 0; i--)
    {
        if (str[i] == '/')
        {   // /, /index.php /asd/index.php?a=b
            //                    5        14
            virgulePos = i;
            // path.first = str.substr(0, virgulePos + 1);   // / / /asd/
            // std::cout << "first:" << path.first << std::endl;
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
    if (virgulePos != -1)
    {
        path.first = str.substr(0, virgulePos + 1);
        if (questionMarkPos != -1 && questionMarkPos - virgulePos > 1)
        {
            path.second = str.substr(virgulePos + 1 , questionMarkPos - virgulePos - 1);
        }
        else if (questionMarkPos == -1 && length - virgulePos > 1)
        {
            path.second = str.substr(virgulePos + 1 , questionMarkPos - virgulePos - 1);
        }
        else
        {
            path.second = "default.html";
        }
    }
    else 
    {
        path.first = "";
        path.second = "";
    }
    std:: cout << "First: " << path.first << std::endl;
    std:: cout << "Second: " << path.second << std::endl;
    // path.first = str.substr(0, virgulePos + 1);
    // path.second = 
    return path;
}

std::map<std::string, std::string> Request::requestParamsAnalyse(std::string str)
{
    std::cout << "params init: " << str << std::endl;
    std::map<std::string, std::string> params;
    // /?123123123
    int length = str.length();
    int leftPos = -1;   //if set, ?  found
    for (int i = 0; i <= length - 2; i++)
    {
        if (str[i] == '?')
        {
            leftPos = i;
            break;
        }
    }
    if (leftPos == -1)
    {
        // No params
        return params;
    }
    // ?123123123\r\n
    std::string paramsStr = str.substr(leftPos + 1, length - 2 - leftPos + 1);
    std::cout << paramsStr << std::endl;

    std::string key = "";
    std::string value = "";

    int paramsStrLength = paramsStr.length();
    for (int i = 0, flag = 0; i < paramsStrLength; i++)
    {
        if (paramsStr[i] == '=')
        {
            flag = 1;
            continue;
        }
        else if (paramsStr[i] == '&' || i == paramsStrLength - 1)
        {
            flag = 2;
        }
        else
        {
            //
        }

        if (flag == 0)
        {
            key += paramsStr[i];
        }
        else if (flag == 1)
        {
            value += paramsStr[i];
        }
        else if (flag == 2)
        {
            if (i == paramsStrLength - 1)
            {
                value += paramsStr[i];
            }
            std::cout << key << ":" << value << std::endl;
            if (key != "")
            {
                params[key] = value;    // there should check repeat, same to cgi/php/wsgi
                std::cout << "get: " << key << "->" << value << std::endl;
            }
            flag = 0;
            key = "";
            value = "";
        }
    }
    return params;
}



std::string Request::getMethod()
{
    if (this->isSetHeaders("requestLine"))
    {
        return this->method;
    }
    else
    {
        return "";
    }
}

std::string Request::getPath()
{
    if (this->isSetHeaders("requestLine"))
    {
        return this->path.first;
    }
    else
    {
        //  error 
        return "";
    }
}


std::string Request::getFile()
{
    if (this->isSetHeaders("requestLine"))
    {
        return this->path.second;
    }
    else
    {
        //  error 
        return "";
    }
}


std::map<std::string, std::string> Request::getParams()
{
    return this->params;
}


std::string Request::getVersion()
{
    if (this->version != "")
    {
        return this->version;
    }
    else
    {
        //  error 
        return "";
    }
}

std::string Request::getHeaderByName(std::string str)
{
    std::map<std::string, std::string>::iterator iter;
    iter = this->params.find(str);
    if (iter != this->params.end())
    {
        return iter->second;
    }
    else
    {
        return "";
    }
}


std::map<std::string, std::string> Request::getCookies()
{
    return this->Cookie;
}

