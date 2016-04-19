/*
 * Http.h
 *
 *  Created on: 2016年4月16日
 *      Author: lc4t
 */
#ifndef HTTP_H_
#define HTTP_H_
#include <cstring>
#include <exception>
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
        // Request Headers
        std::string method; // 
        std::pair<std::string, std::string> path;   // <path, file>
        std::map<std::string, std::string> params;
        std::string version;

        int requestlength;                             // current length, contain \r\n
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

        // Response Headers
        std::string RES_Cache_Control;
        std::string RES_Connection;
        std::string RES_Content_Language;
        std::string RES_Content_Type;//:text/html; charset=utf-8
        std::string RES_Date;//:Sun, 17 Apr 2016 15:06:00 GMT
        std::string RES_ETag;//:"1460905560"
        std::string RES_Expires;//:Sun, 19 Nov 1978 05:00:00 GMT
        std::string RES_Keep_Alive;//:timeout=5, max=100
        std::string RES_Last_Modified;//:Sun, 17 Apr 2016 15:06:00 GMT
        std::string RES_Server;//:Apache/2.2.26 (Unix) mod_ssl/2.2.26 OpenSSL/1.0.1e-fips mod_mono/2.6.3 mod_auth_passthrough/2.1 mod_bwlimited/1.4
        std::string RES_Transfer_Encoding;//:chunked
        std::string RES_X_Powered_By;//:PHP/5.4.24
        // Response end



        bool setRequestLine(std::string requestLine);
        bool isSet(std::string);
        std::string getMethod();

        std::pair<std::string, std::string> requestPathAnalyse(std::string str);
        std::map<std::string, std::string> requestParamsAnalyse(std::string str);
};


#endif /* SERVER_H_ */