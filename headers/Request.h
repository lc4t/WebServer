/*
 * Request.h
 *
 *  Created on: 2016年4月16日
 *      Author: lc4t
 */
#ifndef REQUEST_H_
#define REQUEST_H_
#include <assert.h>
#include <cstring>
#include <exception>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <stdio.h>

class Request
{
    public:
        Request();
        bool addHeader(std::string str);
        // FILE* analyse(std::string receiveStr);
        // bool getStatus();
        bool Test();
        bool isEnd();
        std::string getMethod();
        std::string getPath();
        std::string getFile();
        std::map<std::string, std::string> getParams();
        std::string getVersion();
        std::string getHeaderByName(std::string str);
        std::map<std::string, std::string> getCookies();
    private:
        // Request Headers
        std::string method; // 
        std::pair<std::string, std::string> path;   // <path, file>
        std::map<std::string, std::string> params;
        std::string version;

        int requestlength;                             // current length, contain \r\n
        // std::map<std::string, int> headers;    // is set
        std::set<std::string> headers;
        std::string Host;
        std::string User_Agent;
        std::string Accept;
        std::string Accept_Language;
        std::string Accept_Encoding;
        std::string Accept_Charset;
        std::string Keep_Alive;
        std::string Connection;
        std::map<std::string, std::string> Cookie;
        std::string Pragma;
        std::string Cache_Control;
        std::string Upgrade_Insecure_Requests;
        bool doubleRN;

        // POST params
        // Request end
        bool setRequestLine(std::string requestLine);
        bool isSetHeaders(std::string key);
        // std::string getMethod();

        std::pair<std::string, std::string> requestPathAnalyse(std::string str);
        std::map<std::string, std::string> requestParamsAnalyse(std::string str);

        bool status;

};


#endif /* REQUEST_H_ */