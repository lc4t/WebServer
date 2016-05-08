#ifndef PHP_H_
#define PHP_H_

#include "../config.hpp"

#include "Page.h"
class CGI : public Page
{
    public:
        CGI(std::string version, std::string dir, std::string filename, std::string contentType, std::string method, std::string rawParams);


};

#endif
