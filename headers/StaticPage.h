#ifndef STATICPAGE_H_
#define STATICPAGE_H_

#include "../config.hpp"

#include "Page.h"

class StaticPage: public Page
{
    public:
        StaticPage(std::string version, std::string dir, std::string filename, std::string contentType);


        FILE* getPage();
        char* getResponseHeader();

    private:



        // init
        int status;

        // set response headers
        char* responseHeadersStr;
        int setPage();  // first setPage and then know what Response Headers to response
        int setResponseHeader();

        // set page
        FILE* page;


};


 #endif
