/*
 * Http.h
 *
 *  Created on: 2016年4月16日
 *      Author: lc4t
 */
#ifndef HTTP_H_
#define HTTP_H_
#include <cstring>
#include <iostream>
#include <map>
#include <stdio.h>

class Http
{
    public:
        Http();
        bool addHeader(std::string str);
        FILE* analyse(std::string receiveStr);
        // bool getStatus();
        bool isEnd();

        // void pushR();
        // void pushN();
    private:
        // Request
        std::string method; // 
        std::pair<std::string, std::string> path;   // <path, file>
        std::map<std::string, std::string> params;
        std::string version;

        int length;                             // current length, contain \r\n
        std::map<std::string, bool> headers;    // is set
        
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

        // Response
        
        // Response end

        // typedef struct requestLineStruct
        // {
        //     std::string method; // 
        //     std::pair<std::string, std::string> path;   // <path, file>
        //     std::map<std::string, std::string> params;
        //     std::string version;
        // }requestLineStruct;
        // typedef struct httpRequestStruct
        // {
        //     httpRequestStruct()
        //     {
        //         requestLine = new requestLineStruct
        //     }
        //     int length;                             // current length, contain \r\n
        //     std::map<std::string, bool> headers;    // is set

        //     requestLineStruct* requestLine;
        //     std::string Host;
        //     std::string User_Agent;
        //     std::string Accept;
        //     std::string Accept_Language;
        //     std::string Accept_Encoding;
        //     std::string Accept_Charset;
        //     std::string Keep_Alive;
        //     std::string Connection;
        //     std::map<std::string, std::string> Cookie;
        //     std::string Pragma;
        //     std::string Cache_Control;
        //     std::string Upgrade_Insecure_Requests;
        //     bool doubleRN;

        //     // POST params
        // }httpRequestStruct;

        // httpRequestStruct* httpRequest;

        bool setRequestLine(std::string requestLine);
        bool isSet(std::string);
        std::string getMethod();

        std::pair<std::string, std::string> requestPathAnalyse(std::string str);
        std::map<std::string, std::string> requestParamsAnalyse(std::string str);
};


#endif /* SERVER_H_ */