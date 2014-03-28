#ifndef MULTISPRITE__H
#define MULTISPRITE__H
#include <string>
#include <vector>
#include "drawable.h"

class MultiSprite : public Drawable {
public:
  MultiSprite(const std::string&);
  MultiSprite(const MultiSprite&);
  virtual ~MultiSprite() { } 

  MultiSprite& operator=(const MultiSprite&);
  virtual void draw() const;
  virtual void scaledDraw(double dx, double dy) const;
  virtual void update(Uint32 ticks);

  virtual Vector2f getSize() const { return Vector2f(frameWidth,frameHeight); }

  int getH() const { return frameHeight; }
  int getW() const { return frameWidth; }

  virtual const Frame* getFrame() const { 
    return frames[currentFrame]; 
  }

protected:
  const std::vector<Frame *> frames;
  int worldWidth;
  int worldHeight;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int frameWidth;
  int frameHeight;

  void advanceFrame(Uint32 ticks);
};
#endif
