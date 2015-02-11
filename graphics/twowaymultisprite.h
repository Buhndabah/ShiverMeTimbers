#ifndef TWOWAYMULTISPRITE__H
#define TWOWAYMULTISPRITE__H
#include <string>
#include <vector>
#include "multisprite.h"

class TwoWayMultiSprite : public MultiSprite {
public:
  TwoWayMultiSprite(const std::string&);
  TwoWayMultiSprite(const TwoWayMultiSprite&);
  virtual ~TwoWayMultiSprite() {}

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const {
    if(!swap)
	return frames[currentFrame]
    else
	return swapFrames[currentFrame]
  }

protected:
  const std::vector<Frame *> swapFrames;
  unsigned numberOfSwapFrames;
  unsigned swapFrameInterval;
  int swapFrameWidth;
  int swapFrameHeight;
};
