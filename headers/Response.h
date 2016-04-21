/*
 * Response.h
 *
 *  Created on: 2016年4月19日
 *      Author: lc4t
 */

#ifndef RESPONSE_H_
#define RESPONSE_H_

#include <assert.h>
#include "Request.h"

class Response
{
    public:
        Response();
        // Response(Request request);
        Request* getRequest();
        

        char* getResponseHeaders();
        FILE* getPage();
    private:
        // Response Headers
        std::string Cache_Control;
        std::string Connection;
        std::string Content_Language;
        std::string Content_Type;//:text/html; charset=utf-8
        std::string Date;//:Sun, 17 Apr 2016 15:06:00 GMT
        std::string ETag;//:"1460905560"
        std::string Expires;//:Sun, 19 Nov 1978 05:00:00 GMT
        std::string Keep_Alive;//:timeout=5, max=100
        std::string Last_Modified;//:Sun, 17 Apr 2016 15:06:00 GMT
        std::string Server;//:Apache/2.2.26 (Unix) mod_ssl/2.2.26 OpenSSL/1.0.1e-fips mod_mono/2.6.3 mod_auth_passthrough/2.1 mod_bwlimited/1.4
        std::string Transfer_Encoding;//:chunked
        std::string X_Powered_By;//:PHP/5.4.24
        // Response end
        int status;
        bool setPage();
        FILE* page;
        char* status200();
        char* status403();
        char* status404();
        char* status500();
        char* responseStr;

        Request* request;
};


 #endif /* RESPONSE_H_ */