#include <cstring>
#include <sstream>
#include <limits>
#include "gamedata.h"

Gamedata& Gamedata::getInstance() {
  static Gamedata instance;
  return instance;
}

Gamedata::Gamedata() :
  parser(XMLParser::getInstance()),
  readFile("xmlSpec/game.xml"),
  roto(false)
{
    parser.parse(readFile);
}

float Gamedata::getRandInRange(int min, int max) const {
  return min + (rand()/(std::numeric_limits<int>::max()+1.0f))*(max-min);
}

bool Gamedata::getXmlBool(const std::string& tag) const {
    parser.setCurDocument(readFile);
    return (strcmp(std::string(parser.find_value(tag)).c_str(),"true") ? false:true);
}

int Gamedata::getXmlInt(const std::string& tag) const {
    parser.setCurDocument(readFile);
    std::stringstream strm;
    strm<< std::string(parser.find_value(tag));
    int data;
    strm >> data;
    return data;
}

float Gamedata::getXmlFloat(const std::string& tag) const {
    parser.setCurDocument(readFile);
    std::stringstream strm;
    strm << std::string(parser.find_value(tag));
    float data;
    strm >> data;
    return data;
}

const std::string Gamedata::getXmlStr(const std::string& tag) const {
    parser.setCurDocument(readFile);
    return parser.find_value(tag);
}

void Gamedata::displayData() const {
    parser.setCurDocument(readFile);
    parser.displayData();
}


