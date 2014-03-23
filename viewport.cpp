#include <sstream>
#include "ioManager.h"
#include "viewport.h"
#include "hud.h"

Viewport& Viewport::getInstance() {
  static Viewport viewport;
  return viewport;
}

Viewport::Viewport() : 
  hud(HUD::getInstance()),
  gdata(Gamedata::getInstance()),
  position(0, 0),
  viewWidth(gdata.getXmlInt("viewWidth")), 
  viewHeight(gdata.getXmlInt("viewHeight")),
  worldWidth(gdata.getXmlInt("worldWidth")),
  worldHeight(gdata.getXmlInt("worldHeight")),
  objWidth(0), objHeight(0),
  objectToTrack(NULL) 
{ 
    hud.addTextComponent("trackObj", Vector2f(0,30),"", true);
}

void Viewport::setObjectToTrack(const Drawable *obj) { 
  objectToTrack = obj; 
  objWidth = objectToTrack->getFrame()->getWidth();
  objHeight = objectToTrack->getFrame()->getHeight();
}

void Viewport::draw() const {
    hud.setComponentText("trackObj", "Tracking "+objectToTrack->getName());
  //IOManager::getInstance().
    //printMessageCenteredAt("Tracking "+objectToTrack->getName(), 30);
}

void Viewport::update() {
  const float x = objectToTrack->X();
  const float y = objectToTrack->Y();
  position[0] = (x + objWidth/2) - viewWidth/2;
  position[1] = (y + objHeight/2) - viewHeight/2;
  if (position[0] < 0) position[0] = 0;
  if (position[1] < 0) position[1] = 0;
  if (position[0] > (worldWidth - viewWidth)) {
    position[0] = worldWidth-viewWidth;
  }
  if (position[1] > (worldHeight - viewHeight)) {
    position[1] = worldHeight-viewHeight;
  }
}
