/*
 * Http.cpp
 *
 *  Created on: 2016年4月16日
 *      Author: lc4t
 */

#include "../headers/Http.h"

Http::Http()
{
    this -> length = 0;
    this -> doubleRN = false;
}

bool Http::addHeader(std::string str)
{

    // std::cout << "Header(" << str.length() << ") ";
    // std::cout << "(" << str << ")" << std::endl;

    if (str == "\r\n")
    {
        std::cout << "receive: \\r\\n" << std::endl;
        this -> doubleRN = true;
        return true;
    }
    if (this -> isSet("requestLine"))
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
                
                // std::cout << "<" << key << ">"<< std::endl;
                // std::cout << "<" << value << ">" << std::endl;
                if (key == "Host")
                {
                    this -> Host = value; 
                    this -> headers[key] = true;

                    // std::cout << "set " << key <<": " << value << std::endl;
                    return true;
                }
                else if (key == "User-Agent" )
                {
                    this -> User_Agent = value; 
                    this -> headers[key] = true;

                    // std::cout << "set " << key << ": " << value << std::endl;
                    return true;
                }
                else if (key == "Accept")
                {
                    this -> Accept = value; 
                    this -> headers[key] = true;

                    // std::cout << "set " << key << ": " << value << std::endl;
                    return true;
                }
                else if (key == "Accept-Language" )
                {
                    this -> Accept_Language = value; 
                    this -> headers[key] = true;

                    // std::cout << "set " << key << ": " << value << std::endl;
                    return true;
                }
                else if (key == "Accept-Encoding")
                {
                    this -> Accept_Encoding = value; 
                    this -> headers[key] = true;

                    // std::cout << "set " << key << ": " << value << std::endl;
                    return true;
                }
                else if (key == "Keep-Alive")
                {
                    this -> Keep_Alive = value; 
                    this -> headers[key] = true;

                    // std::cout << "set " << key << ": " << value << std::endl;
                    return true;
                }
                else if (key == "Connection" )
                {
                    this -> Connection = value; 
                    this -> headers[key] = true;

                    // std::cout << "set " << key << ": " << value << std::endl;
                    return true;
                }
                else if (key == "Pragma" )
                {
                    this -> Pragma = value; 
                    this -> headers[key] = true;

                    // std::cout << "set " << key << ": " << value << std::endl;
                    return true;
                }
                else if (key == "Cache-Control")
                {
                    this -> Cache_Control = value; 
                    this -> headers[key] = true;

                    // std::cout << "set " << key << ": " << value << std::endl;
                    return true;
                }
                else if (key == "Upgrade-Insecure-Requests")
                {
                    this -> Upgrade_Insecure_Requests = value; 
                    this -> headers[key] = true;

                    // std::cout << "set " << key << ": " << value << std::endl;
                    return true;
                }
                else if (key == "cookie")
                {
                    //
                    // this -> Cookie['Cookie'] = formatCookie(value);
                    std::cout << "cookie " << value << std::endl;
                    this -> headers[key] = true;
                    return true;
                }
                else
                {
                    std::cout << "Unknow headers." << std::endl;
                    return false;
                }
            }
        }
    }
    else
    {
        return this -> setRequestLine(str);
    }
}

FILE* Http::analyse(std::string receiveStr)
{
    // std::cout << receiveStr.length() << std::endl;
    // std::cout << receiveStr << std::endl;
    char filename[] = "/home/lc4t/Documents/git/WebServer/html/200.html";
    FILE* fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        std::cout << "Cannot open file " << filename << std::endl;
    }
    return fp;
}


// bool Http::getStatus()
// {
//     return false;
// }


bool Http::isEnd()
{
    if (this -> getMethod() == "GET")
    {
        return this -> doubleRN;
    }
    return true;
}

bool Http::setRequestLine(std::string requestLine)
{
    std::cout << "<*" << requestLine << "*>" << std::endl;
    int length = requestLine.length();
    std::string split[3];
    for (int leftPos = 0, rightPos = 0, count = 0; rightPos < length, count < 3; rightPos++) 
    // count is make to split, left right pos is to find a slipt
    {
        if (requestLine[rightPos] == ' ' || requestLine[rightPos] == '\r')
        { 
            split[count++] = requestLine.substr(leftPos, rightPos - leftPos);
            leftPos = rightPos + 1;
            // std::cout << split[count - 1] << std::endl;
        }
    }
    this -> method = split[0];
    this -> path = this -> requestPathAnalyse(split[1]);  
    this -> params = this -> requestParamsAnalyse(split[1]);  
    this -> version = split[2];
    this->headers["requestLine"] = true;
    return true;
}


bool Http::isSet(std::string key)
{
    return (this->headers.find(key) != this->headers.end());
}


std::string Http::getMethod()
{
    if (isSet("requestLine"))
    {
        return this -> method;
    }
    else
    {
        return "";
    }
}


std::pair<std::string, std::string> Http::requestPathAnalyse(std::string str)
{  
    std::pair<std::string, std::string> path;
    int length = str.length();
    int questionMarkPos = -1;// = str.length() - 1 - 2;  // ? pass \r\n
    int virgulePos = -1;//= str.length() - 1 - 2;      // /
    for (int i = length - 1 - 2; i >= 0; i--)
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
        
    }
    if (virgulePos != -1)
    {
        path.first = str.substr(0, virgulePos + 1);
        if (questionMarkPos != -1 && questionMarkPos - virgulePos > 1)
        {
            path.second = str.substr(questionMarkPos + 1 , questionMarkPos - virgulePos);
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

std::map<std::string, std::string> Http::requestParamsAnalyse(std::string str)
{
    std::cout << "params: " << str << std::endl;
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