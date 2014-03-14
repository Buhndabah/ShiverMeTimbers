#ifndef ROTATESPRITE__H
#define ROTATESPRITE__H
#include <string>
#include <vector>
#include "drawable.h"

class RotateSprite : public Drawable {
public:
  RotateSprite(const std::string&);
  RotateSprite(const RotateSprite&);
  virtual ~RotateSprite() {}

  RotateSprite& operator=(const RotateSprite&);

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const {
    return frame;
  }

protected:
  const Frame * frame;
  int worldWidth;
  int worldHeight;
  int frameWidth;
  int frameHeight;
  double angle;
  double deltaAngle;
};
#endif
