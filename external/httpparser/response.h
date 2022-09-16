/*
 * Copyright (C) Alex Nekipelov (alex@nekipelov.net)
 * License: MIT
 */

#ifndef HTTPPARSER_RESPONSE_H
#define HTTPPARSER_RESPONSE_H

#include <string>
#include <vector>
#include <sstream>

namespace httpparser
{

struct Response {
    Response()
        : versionMajor(1), versionMinor(1), keepAlive(false), statusCode(0)
    {}
    
    struct HeaderItem
    {
        std::string name;
        std::string value;
    };

    int versionMajor;
    int versionMinor;
    std::vector<HeaderItem> headers;
    std::vector<char> content;
    bool keepAlive;
    
    unsigned int statusCode;
    std::string status;

    std::string inspect() const
    {
        std::stringstream stream;
        stream << "HTTP/" << versionMajor << "." << versionMinor
               << " " << statusCode << " " << status << "\n";

        for(std::vector<Response::HeaderItem>::const_iterator it = headers.begin();
            it != headers.end(); ++it)
        {
            stream << it->name << ": " << it->value << "\n";
        }

        std::string data(content.begin(), content.end());
        stream << data << "\n";
        return stream.str();
    }

    std::string serialize() const
    {
        std::stringstream stream;
        stream << "HTTP/" << versionMajor << "." << versionMinor
               << " " << statusCode << " " << status << std::endl;

        for(std::vector<Response::HeaderItem>::const_iterator it = headers.begin();
            it != headers.end(); ++it)
        {
            stream << it->name << ": " << it->value << std::endl;
        }

        std::string data(content.begin(), content.end());
        stream << std::endl << data;
        return stream.str();
    }

    std::string content_as_str(){
        std::string str;

        for(auto ch: content){
            str += ch;
        }
        
        return str;
    }

    void str_to_content(std::string str){
        Response::HeaderItem cont_len{"Content-Length",
                                  std::to_string(str.length())};
        headers.push_back(cont_len);

        content.clear();
        for(auto ch: str){
            content.push_back(ch);
        }
    }
    
};

} // namespace httpparser

#endif // HTTPPARSER_RESPONSE_H

