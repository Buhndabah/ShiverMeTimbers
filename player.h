#ifndef PLAYER__H
#define PLAYER__H
#include <string>
#include <vector>
#include "vector2f.h"
#include "multisprite.h"

class Player : public MultiSprite{
public:
  Player(const std::string&);
  Player(const Player&);
  virtual ~Player() {}

//  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const {
    return MultiSprite::getFrame();
  }

  void moveUp();
  void moveUpLeft();
  void moveUpRight();
  void moveDown();
  void moveDownLeft();
  void moveDownRight();
  void moveLeft();
  void moveRight();
  void stop();

  float gridX() const { return gridPosition[0]; }
  void gridX(float x) { gridPosition[0] = x; }

  float gridY() const { return gridPosition[1]; }
  void gridY(float y) { gridPosition[1] = y; }

  float gridVelocityX() const { return gridVelocity[0]; }
  void gridVelocityX(float x) { gridVelocity[0] = x; }

  float gridVelocityY() const { return gridVelocity[1]; }
  void gridVelocityY(float y) { gridVelocity[1] = y; }

  void addGVX(float x) { gridVelocity[0] += x; }
  void addGVY(float y) { gridVelocity[1] += y; }

  const Vector2f& getGridPosition() const   { return gridPosition; }
  void setGridPosition(const Vector2f& pos) { gridPosition = pos; }
  const Vector2f& getGridVelocity() const   { return gridVelocity; }
  void setGridVelocity(const Vector2f& vel) { gridVelocity = vel; }

protected:
  Vector2f gridPosition;
  Vector2f gridVelocity;
  float moveSpeed;
};

#endif
