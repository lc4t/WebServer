#include "../headers/PHP.h"

PHP::PHP(std::string version, std::string dir, std::string filename, std::string contentType)
{
    std::cout << "41#PHPtest:" << dir << filename << contentType << std::endl;
}

// from PHP 5.2.17
int PHP::php_htoi(char *s)
{
    int value;
    int c;

    c = ((unsigned char *)s)[0];
    if (isupper(c))
    {
        c = tolower(c);
    }

    value = (c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10) * 16;

    c = ((unsigned char *)s)[1];
    if (isupper(c))
    {
        c = tolower(c);
    }

    value += c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10;

    return (value);
}

// from PHP 5.2.17
std::string PHP::urldecode(std::string &str_source)
{
    char const *in_str = str_source.c_str();
    int in_str_len = strlen(in_str);
    int out_str_len = 0;
    std::string out_str;
    char *str;

    str = strdup(in_str);
    char *dest = str;
    char *data = str;

    while (in_str_len--)
    {
        if (*data == '+')
        {
            *dest = ' ';
        }
        else if (*data == '%' && in_str_len >= 2 && isxdigit((int) *(data + 1)) && isxdigit((int) *(data + 2)))
        {
            *dest = (char) php_htoi(data + 1);
            data += 2;
            in_str_len -= 2;
        }
        else
        {
            *dest = *data;
        }
        data++;
        dest++;
    }
    *dest = '\0';
    out_str_len =  dest - str;
    out_str = str;
    free(str);
    return out_str;
}
