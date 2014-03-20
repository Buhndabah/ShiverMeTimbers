#include <sstream>
#include <limits>
#include "gamedata.h"

Gamedata& Gamedata::getInstance() {
  static Gamedata instance;
  return instance;
}

Gamedata::Gamedata(const std::string& fn ) :
  xmlData(fn),
  roto(false)
{ }

float Gamedata::getRandInRange(int min, int max) const {
  return min + (rand()/(std::numeric_limits<int>::max()+1.0f))*(max-min);
}

bool Gamedata::getXmlBool(const std::string& tag) const {
  return xmlData.getXmlBool(tag);
}

int Gamedata::getXmlInt(const std::string& tag) const {
  return xmlData.getXmlInt(tag);
}

float Gamedata::getXmlFloat(const std::string& tag) const {
  return xmlData.getXmlFloat(tag);
}

const std::string Gamedata::getXmlStr(const std::string& tag) const {
  return xmlData.getXmlStr(tag);
}

void Gamedata::displayData() const {
    xmlData.displayData();
}


