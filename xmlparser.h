#ifndef XMLPARSER_H
#define XMLPARSER_H
#include <fstream>
#include <iterator>
#include <vector>
#include <iostream>
#include "rapidxml.h"

class XMLParser {
public:
    XMLParser() :
        doc(),
        file(),
        buf()
    { }

    XMLParser(const std::string& fn) :
        doc(),
        file(fn.c_str(),std::ifstream::in),
        buf(std::istreambuf_iterator<char>(file),std::istreambuf_iterator<char>())
    { 
        buf.push_back('\0');
        doc.parse<0>(&buf[0]);
    }

    const char* find_value(const std::string& tag) const;

    void displayData() const;

private:
    rapidxml::xml_document<> doc;
    std::ifstream file;
    std::vector<char> buf;  // doc retains references to the input vector, so this needs to be a member
};
#endif
