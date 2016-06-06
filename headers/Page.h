#ifndef PAGE_H_
#define PAGE_H_

#include "../config.hpp"

class Page
{
    public:
        Page();
        std::string ostreamType;
        char* getResponseHeader();
        void* getPage();    // need override
    protected:
        // target
        std::string dir;
        std::string filename;

        // response headers
        char* responseHeadersStr;
        std::map<std::string, std::string> headers;
        int setResponseHeader();

        // this page
        int setPage();




};

#endif
