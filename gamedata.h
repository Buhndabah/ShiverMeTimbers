#ifndef GAMEDATA__H
#define GAMEDATA__H
#include <string>
#include "xmldata.h"

class Gamedata {
public:
  static Gamedata& getInstance();
  void displayData() const;

  bool getXmlBool(const std::string&) const;
  const std::string getXmlStr(const std::string&) const;
  float getXmlFloat(const std::string&) const;
  int getXmlInt(const std::string&) const;
  bool getRoto() const {return roto;}

  void setRoto(bool r) {roto = r;}

  // An auxilliary function:
  float getRandInRange(int min, int max) const;
private:
  Xmldata xmlData;
  bool roto;

  Gamedata(const std::string& fn = "xmlSpec/game.xml");
  Gamedata(const Gamedata&);
  Gamedata& operator=(const Gamedata&);
};
#endif
