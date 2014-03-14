#include "SDL/SDL_rotozoom.h"
#include "ioManager.h"
#include "frame.h"
#include "viewport.h"

// The next constructor s/ only be used when the Frames's surface
// starts at (0, 0) in the sprite sheet, and the frame uses the 
// entire sheet. For example, a Frame for an orb or a background.
Frame::Frame( const std::string& name, SDL_Surface* surf ) : 
  screen(IOManager::getInstance().getScreen()),
  width(Gamedata::getInstance().getXmlInt(name+"Width")), 
  height(Gamedata::getInstance().getXmlInt(name+"Height")),
  surface( surf )
{ }

Frame& Frame::operator=(const Frame& rhs) {
  if ( this == &rhs ) return *this;
  screen = rhs.screen;
  width = rhs.width;
  height = rhs.height;
  surface = rhs.surface;
  return *this;
}

void Frame::draw(Sint16 x, Sint16 y) const {
  x -= Viewport::getInstance().X();
  y -= Viewport::getInstance().Y();
  SDL_Rect src = { 0, 0, width, height };    
  SDL_Rect dest = {x, y, width, height };
  SDL_BlitSurface(surface, &src, screen, &dest);
}

void Frame::draw(Sint16 sx, Sint16 sy, Sint16 dx, Sint16 dy) const {
  //x -= Viewport::getInstance().X();
  //y -= Viewport::getInstance().Y();
  SDL_Rect src = { sx, sy, width, height };    
  SDL_Rect dest = {dx, dy, width, height };
  SDL_BlitSurface(surface, &src, screen, &dest);
}

void Frame::draw(Sint16 x, Sint16 y, double angle) const {
  x -= Viewport::getInstance().X();
  y -= Viewport::getInstance().Y();
  SDL_Surface* tmp = rotozoomSurface(surface, angle, 1, 1);
  SDL_Rect src = { 0, 0, tmp->w, tmp->h };    
  SDL_Rect dest = {x, y, 0, 0 };
  SDL_BlitSurface(tmp, &src, screen, &dest);
  SDL_FreeSurface( tmp );
}
