#include <cmath>
#include "sprite.h"
#include "gamedata.h"
#include "frameFactory.h"

Sprite::Sprite(const std::string& name) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"X"), 
                    Gamedata::getInstance().getXmlInt(name+"Y")), 
           //Vector2f(Gamedata::getInstance().getXmlInt(name+"SpeedX"), 
           //         Gamedata::getInstance().getXmlInt(name+"SpeedY")) 
           Vector2f(rand() % 400 - 300, 
                    rand() % 400 - 300) 

           ),
  frame( FrameFactory::getInstance().getFrame(name) ),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("worldWidth")),
  worldHeight(Gamedata::getInstance().getXmlInt("worldHeight"))
{ }

Sprite::Sprite(const std::string& n, const Vector2f& pos, const Vector2f& vel):
  Drawable(n, pos, vel), 
  frame( FrameFactory::getInstance().getFrame(n) ),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("worldWidth")),
  worldHeight(Gamedata::getInstance().getXmlInt("worldHeight"))
{ }

Sprite::Sprite(const std::string& n, const Vector2f& pos, const Vector2f& vel,
               const Frame* frm):
  Drawable(n, pos, vel), 
  frame( frm ),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("worldWidth")),
  worldHeight(Gamedata::getInstance().getXmlInt("worldHeight"))
{ }

Sprite::Sprite(const Sprite& s) :
  Drawable(s), 
  frame(s.frame),
  frameWidth(s.getFrame()->getWidth()),
  frameHeight(s.getFrame()->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("worldWidth")),
  worldHeight(Gamedata::getInstance().getXmlInt("worldHeight"))
{ }

Sprite& Sprite::operator=(const Sprite& rhs) {
  Drawable::operator=( rhs );
  frame = rhs.frame;
  frameWidth = rhs.frameWidth;
  frameHeight = rhs.frameHeight;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  return *this;
}

void Sprite::draw() const {
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frame->draw(x, y); 
}

int Sprite::getDistance(const Sprite *obj) const { 
  return hypot(X()-obj->X(), Y()-obj->Y());
}

void Sprite::update(Uint32 ticks) { 
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( Y() < 0) {
    velocityY( abs( velocityY() ) );
  }
  if ( Y() > worldHeight-frameHeight) {
    velocityY( -abs( velocityY() ) );
  }

  if ( X() < 0) {
    velocityX( abs( velocityX() ) );
  }
  if ( X() > worldWidth-frameWidth) {
    velocityX( -abs( velocityX() ) );
  }  
}
