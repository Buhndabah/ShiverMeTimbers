#ifndef XMLDATA_H
#define XMLDATA_H
#include <string>
#include <map>
#include "xmlparser.h"

class Xmldata {
public:
    Xmldata(const std::string& fn);
    ~Xmldata() {} 
    void displayData() const;
    bool getXmlBool(const std::string&) const;
    const std::string getXmlStr(const std::string&) const;
    float getXmlFloat(const std::string&) const;
    int getXmlInt(const std::string&) const;


private:
    Xmldata& operator=(const Xmldata&);
    Xmldata(const Xmldata&);
    XMLParser parser;
};
#endif
