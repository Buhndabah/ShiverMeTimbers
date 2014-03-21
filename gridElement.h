#ifndef GRIDELEMENT__H
#define GRIDELEMENT__H
#include <string>
#include <vector>
#include "vector2f.h"
//#include "drawable.h"
#include "multisprite.h"

class GridElement {
public:
  GridElement(const std::string&);
  GridElement(const GridElement&);
  virtual ~GridElement() {}

//  GridElement& operator=(const GridElement&);
  MultiSprite& getSprite(){
    return gridSprite;
  }
 
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


  void update(Uint32 ticks);
  void moveUp();
  void moveUpLeft();
  void moveUpRight();
  void moveDown();
  void moveDownLeft();
  void moveDownRight();
  void moveLeft();
  void moveRight();
  void stop();


private:
//  Drawable* gridSprite;
  float moveSpeed;
  MultiSprite gridSprite;
  Vector2f gridPosition;
  Vector2f gridVelocity;
};

#endif
