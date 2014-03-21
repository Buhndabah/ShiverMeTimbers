#ifndef XMLDATA_H
#define XMLDATA_H
#include <string>
#include <map>
#include "xmlparser.h"

class Xmldata {
public:
    Xmldata() : parser() {}

    Xmldata(const std::string& fn);
        
    void displayData() const;
    bool getXmlBool(const std::string&) const;
    const std::string getXmlStr(const std::string&) const;
    float getXmlFloat(const std::string&) const;
    int getXmlInt(const std::string&) const;


private:
    XMLParser parser;
};
#endif
