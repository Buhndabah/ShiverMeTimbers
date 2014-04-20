#include "multisprite.h"
#include "gamedata.h"
#include "frameFactory.h"

void MultiSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

MultiSprite::MultiSprite( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"X"), 
                    Gamedata::getInstance().getXmlInt(name+"Y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"SpeedX"),
                    Gamedata::getInstance().getXmlInt(name+"SpeedY"))
           ),
  frames( FrameFactory::getInstance().getFrames(name) ),
  worldWidth(Gamedata::getInstance().getXmlInt("worldWidth")),
  worldHeight(Gamedata::getInstance().getXmlInt("worldHeight")),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"Frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"FrameInterval") ),
  timeSinceLastFrame( 0 ),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight())
{}

MultiSprite::MultiSprite(const MultiSprite& s) :
  Drawable(s), 
  frames(s.frames),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  frameWidth( s.frameWidth ),
  frameHeight( s.frameHeight )
  { }

MultiSprite& MultiSprite::operator=(const MultiSprite& rhs) {
    Drawable::operator=(rhs);
    frames = std::vector<Frame*>(rhs.frames);
    worldWidth = rhs.worldWidth;
    worldHeight = rhs.worldHeight;
    currentFrame = rhs.currentFrame;
    numberOfFrames = rhs.numberOfFrames;
    frameInterval = rhs.frameInterval;
    timeSinceLastFrame = rhs.timeSinceLastFrame;
    frameWidth = rhs.frameWidth;
    frameHeight = rhs.frameHeight;
    return *this;
}

void MultiSprite::draw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frames[currentFrame]->draw(x, y);
}

void MultiSprite::scaledDraw(double dx, double dy) const {
    Uint32 x = static_cast<Uint32>(X());
    Uint32 y = static_cast<Uint32>(X());
    frames[currentFrame]->scaledDraw(x,y,dx,dy);
}

void MultiSprite::update(Uint32 ticks) { 
  advanceFrame(ticks);

  /*Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
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
  } */ 

}
