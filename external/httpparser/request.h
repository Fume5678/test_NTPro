/*
 * Copyright (C) Alex Nekipelov (alex@nekipelov.net)
 * License: MIT
 */

#ifndef HTTPPARSER_REQUEST_H
#define HTTPPARSER_REQUEST_H

#include <string>
#include <vector>
#include <sstream>

namespace httpparser
{

struct Request {
    Request()
        : versionMajor(1), versionMinor(1), keepAlive(false)
    {}
    
    struct HeaderItem
    {
        std::string name;
        std::string value;
    };

    std::string method;
    std::string uri;
    int versionMajor;
    int versionMinor;
    std::vector<HeaderItem> headers;
    std::vector<char> content;
    bool keepAlive;

    std::string inspect() const
    {
        std::stringstream stream;
        stream << method << " " << uri << " HTTP/"
               << versionMajor << "." << versionMinor << std::endl;

        for(std::vector<Request::HeaderItem>::const_iterator it = headers.begin();
            it != headers.end(); ++it)
        {
            stream << it->name << ": " << it->value << std::endl;
        }

        std::string data(content.begin(), content.end());
        stream << data << "\n";
        return stream.str();
    }

    std::string serialize() const
    {
        std::stringstream stream;
        stream << method << " " << uri << " HTTP/"
               << versionMajor << "." << versionMinor << "\r\n";

        for(std::vector<Request::HeaderItem>::const_iterator it = headers.begin();
            it != headers.end(); ++it)
        {
            stream << it->name << ": " << it->value << "\r\n";
        }

        std::string data(content.begin(), content.end());
        stream << "\r\n" << data;
        stream << "\r\n";
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
        Request::HeaderItem cont_len{"Content-Length",
                                  std::to_string(str.length())};
        headers.push_back(cont_len);

        content.clear();
        for(auto ch: str){
            content.push_back(ch);
        }
    }
    
};

} // namespace httpparser


#endif // HTTPPARSER_REQUEST_H
