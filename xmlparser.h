#ifndef XMLPARSER_H
#define XMLPARSER_H
#include <fstream>
#include <iterator>
#include <vector>
#include <iostream>
#include <list>
#include <map>
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
        try { doc.parse<0>(&buf[0]);}
        catch(rapidxml::parse_error e)
        {
            std::cerr << "Parse error for file " << fn << std::endl;
            std::cerr << e.what() << std::endl;
        }
    }

    const char* find_value(const std::string& tag) const;
    std::list<const rapidxml::xml_node<>*> findNodes(const std::string& tag) const;
    std::list<const rapidxml::xml_node<>*> findNodes(const std::string& tag, const rapidxml::xml_node<>* n) const;
    std::map<std::string,std::string> parseNode(const rapidxml::xml_node<>* node) const;
    void displayData() const;

private:
    rapidxml::xml_document<> doc;
    std::ifstream file;
    std::vector<char> buf;  // doc retains references to the input vector, so this needs to be a member
};
#endif
