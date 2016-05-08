#ifndef PHP_H_
#define PHP_H_

#include "../config.hpp"

#include "Page.h"
class PHP : public Page
{
    public:
        PHP(std::string version, std::string dir, std::string filename, std::string contentType);
    private:
        int php_htoi(char *s);
        std::string urldecode(std::string &str_source);
};

#endif
