#ifndef XMLPARSER_H
#define XMLPARSER_H
#include <iterator>
#include <vector>
#include <iostream>
#include <list>
#include <map>
#include "rapidxml.h"

class XMLParser {
public:
    static XMLParser& getInstance();
    ~XMLParser();

    bool parse(const std::string&);
    bool setCurDocument(const std::string&);
    void removeDoc(const std::string&);

    const char* find_value(const std::string& tag) const;
    std::list<const rapidxml::xml_node<>*> findNodes(const std::string& tag) const;
    std::list<const rapidxml::xml_node<>*> findNodes(const std::string& tag, const rapidxml::xml_node<>* n) const;
    std::map<std::string,std::string> parseNode(const rapidxml::xml_node<>* node) const;
    std::list<std::map<std::string, std::string> > parseNodesWithTag(const std::string& tag) const;
    std::list<std::map<std::string, std::string> > parseNodesWithTag(const std::string& tag, const rapidxml::xml_node<>* node) const;
    void displayData() const;

private:
    XMLParser() :
        curDoc(),
        docs()
    { }

    rapidxml::xml_document<> *curDoc;

    XMLParser& operator=(const XMLParser&);
    XMLParser(const XMLParser&);
    std::map<std::string, std::pair<rapidxml::xml_document<>*, std::vector<char> > >docs;
};
#endif
