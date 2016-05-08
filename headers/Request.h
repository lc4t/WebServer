#ifndef REQUEST_H_
#define REQUEST_H_

#include "../config.hpp"



class Request
{
    /*
    *  path = dir/filename
    */
    public:
        Request();
        bool add(std::string str);
        bool isEnd();
        std::string getMethod();
        std::string getDir();
        std::string getFilename();
        std::string getParams();
        std::string getProtocol();
        int setPostParams(std::string str);
        std::map<std::string, std::string> getCookies();
        std::string getPostLine();
        int getContentLength();
        int getCountRN();
    private:
        int requestlength;

        // Request Headers
        std::string method; //

        std::pair<std::string, std::string> setPath(std::string str);
        std::pair<std::string, std::string> path;   // <path, file>

        std::string setParams(std::string str);
        std::string params;         // GET ?[params] string

        std::string version;

        bool setRequestLine(std::string requestLine);

        std::string getHeaderByName(std::string name);
        std::map<std::string, std::string> headers;

        int countRN;


        bool isSetHeaders(std::string key);

};


#endif
