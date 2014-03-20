#include <cstring>
#include <sstream>
#include <iostream>
#include "xmldata.h"

Xmldata::Xmldata(const std::string& fn ) :
    parser(fn)
{ }

bool Xmldata::getXmlBool(const std::string& tag) const {
    return (strcmp(std::string(parser.find_value(tag)).c_str(),"true") ? false:true );
}
/*bool Xmldata::getXmlBool(const string& tag) const {
    std::map<string, string>::const_iterator ptr = xmlData.find(tag);
    if( ptr == xmlData.end() )
        throw string("Xml: Didn't find boolean tag ")+tag+string(" in xml");
    else {
        if( ptr->second == "true") return true;
        else return false;
    }
}*/

int Xmldata::getXmlInt(const std::string& tag) const {
    std::stringstream strm;
    strm << std::string(parser.find_value(tag));
    int data;
    strm >> data;
    return data;
}

/*int Xmldata::getXmlInt(const string& tag) const {
    std::map<string, string>::const_iterator ptr = xmlData.find(tag);
    if(ptr==xmlData.end())
        throw string("Xml: Didn't find integer tag ")+tag+string(" in xml");
    else
    {
        std::stringstream strm;
        strm << ptr->second;
        int data;
        strm >> data;
        return data;
    }
}*/

float Xmldata::getXmlFloat(const std::string& tag) const {
    std::stringstream strm;
    strm << std::string(parser.find_value(tag));
    float data;
    strm >> data;
    return data;
}

/*float Xmldata::getXmlFloat(const string& tag) const {
    std::map<string, string>::const_iterator ptr = xmlData.find(tag);
    if(ptr==xmlData.end())
        throw string("Xml: Didn't find float tag ")+tag+string(" in xml");
    else
    {
        std::stringstream strm;
        strm << ptr->second;
        float data;
        strm >> data;
        return data;
    }
}*/

const std::string Xmldata::getXmlStr(const std::string& tag) const {
    return parser.find_value(tag);
}
/*const string& Xmldata::getXmlStr(const string& tag) const {
    std::map<string, string>::const_iterator ptr = xmlData.find(tag);
    if(ptr == xmlData.end())
        throw string("Game: Didn't find string tag ")+tag+string(" in xml");
    else return ptr->second;
}*/

/*void Xmldata::displayData() const {
    std::map<string, string>::const_iterator ptr = xmlData.begin();
    while(ptr != xmlData.end()) {
        cout << ptr->first << ", " << ptr->second << endl;
        ++ptr;
    }
}*/

void Xmldata::displayData() const {
    parser.displayData();
}
