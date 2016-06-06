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
        std::map<std::string, std::string> getParamsMap();
        std::string getProtocol();
        int setPostParams(std::string str);
        std::map<std::string, std::string> getCookies();
        std::string getPostLine();
        std::string getHeaderByName(std::string name);
        int getContentLength();
        int getCountRN();
        void setPostEnd();
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


        std::map<std::string, std::string> headers;

        int countRN;
        bool postEnd;

        bool isSetHeaders(std::string key);

};


#endif
