#include <iostream>
#include "gamedata.h"
#include "dummyGrid.h"

DummyGrid& DummyGrid::getInstance() {
  static DummyGrid instance;
  return instance;
}

DummyGrid::DummyGrid() :
  w(Gamedata::getInstance().getXmlInt("tilesX")),
  h(Gamedata::getInstance().getXmlInt("tilesY")),
  tileWidth(Gamedata::getInstance().getXmlFloat("tilesWidth"))
{}

