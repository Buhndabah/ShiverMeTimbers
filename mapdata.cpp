#include "mapdata.h"

Mapdata& Mapdata::getInstance() {
    static Mapdata instance;
    return instance;
}

Mapdata::Mapdata(const string& fn) :
    xmlData(fn),
    tileWidth(),
    tileHeight(),
    mapWidth(),
    mapHeight()
{ 
    
    }

bool Mapdata::getXmlBool(const string& tag) const {
    return xmlData.getXmlBool(tag);
}

int Mapdata::getXmlInt(const string& tag) const {
    return xmlData.getXmlInt(tag);
}

float Mapdata::getXmlFloat(const string& tag) const {
    return xmlData.getXmlFloat(tag);
}

const string& Mapdata::getXmlStr(const string& tag) const {
    return xmlData.getXmlStr(tag);
}

void Mapdata::displayData() const {
    parser.displayData();
    //xmlData.displayData();
}

