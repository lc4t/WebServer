/*
 * Response.cpp
 *
 *  Created on: 2016年4月19日
 *      Author: lc4t
 */

#include "../headers/Response.h"

Response::Response()
{
    //
}

Response::Response(Request request)
{
    this -> http = http;
}

Request Response::getRequest()
{
    return this -> request;
}