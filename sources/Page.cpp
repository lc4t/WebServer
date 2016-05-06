#include "../headers/Page.h"

Page::Page()
{
    // init
    this->ostreamType = STRING_POINTER;
    this->responseHeadersStr = new char[MAX_TRANSPORT_STR_LENGTH]();
    std::string dir = EMPTY_STRING;
    std::string filename =EMPTY_STRING;
    // Set Date:
    time_t t =  time(0);
    char timeStr[64];
    strftime(timeStr, sizeof(timeStr), "%a, %d %b %Y %T %Z", localtime(&t));
    this->headers["Date"] = timeStr;

    // Set Server
    this->headers["Server"] = "lc4t's Server Test";

}
