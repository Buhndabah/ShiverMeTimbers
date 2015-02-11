#include "clock.h"
#include "rotatesprite.h"
#include "gamedata.h"
#include "frameFactory.h"

RotateSprite::RotateSprite(const std::string &name) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"X"),
                    Gamedata::getInstance().getXmlInt(name+"Y")),
           Vector2f(Gamedata::getInstance().getXmlInt(name+"SpeedX"),
                    Gamedata::getInstance().getXmlInt(name+"SpeedY"))
	   ),
  frame(FrameFactory::getInstance().getFrame(name) ),
  worldWidth(Gamedata::getInstance().getXmlInt("worldWidth")),
  worldHeight(Gamedata::getInstance().getXmlInt("worldHeight")),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  angle(0),
  deltaAngle(Gamedata::getInstance().getXmlFloat(name+"DeltaAngle"))
  {}

RotateSprite::RotateSprite(const RotateSprite& s) :
  Drawable(s),
  frame(s.frame),
  worldWidth(s.worldWidth),
  worldHeight(s.worldHeight),
  frameWidth(s.frameWidth),
  frameHeight(s.frameHeight),
  angle(s.angle),
  deltaAngle(s.deltaAngle)
  {}

RotateSprite& RotateSprite::operator=(const RotateSprite& rhs) {
  Drawable::operator=(rhs);
  frame = rhs.frame;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  frameWidth = rhs.frameWidth;
  frameHeight = rhs.frameHeight;
  angle = rhs.angle;
  deltaAngle = rhs.deltaAngle;
  return *this;
}

void RotateSprite::draw() const {
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frame->draw(x,y,angle);
}

void RotateSprite::update(Uint32 ticks) {
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);
  double angleIncr = 0.;
  if(Gamedata::getInstance().getRoto())
    angleIncr = deltaAngle * static_cast<float>(ticks) * 0.001;
  angle += angleIncr; 

  if(Y() < 0) {
    velocityY(abs(velocityY()));
  }
  if(Y() > worldHeight-frameHeight) {
    velocityY(-abs( velocityY()));
  }
  if(X() < 0) {
    velocityX(abs(velocityX()));
  }
  if(X() > worldWidth-frameWidth) {
    velocityX(-abs(velocityX()));
  }

}
