#ifndef GRIDELEMENT__H
#define GRIDELEMENT__H
#include <string>
#include <vector>
#include "vector2f.h"
#include "multisprite.h"
#include "mapManager.h"

class MapManager;

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

  const Vector2f& getPosition() const { return gridSprite.getPosition(); }

  float getMoveSpeed() const {return moveSpeed;}
 
  int getCurrentHP() const { return curHP; }
  int getMaxHP() const { return maxHP; }

  std::vector<bool> getMoveDir() const { return moveDir;}

  std::vector<Vector2f> getMoveboxVertices() const { return moveboxVertices;}
  Vector2f getMoveboxVertex(int indx) const { return moveboxVertices[indx];}
  void setMoveboxVertex(int, Vector2f);

  void onDamage(int damage);

  void draw() const;
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
  void clearMoveDir();

  float moveSpeed;
  MultiSprite gridSprite;
  Vector2f gridPosition;
  Vector2f gridVelocity;
  int maxHP;
  int curHP;
  const MapManager& map; //the grid the gridElement is attached to
  std::vector<bool> moveDir; //boolean vector telling the gridElement which direction it is moving in
  std::vector<Vector2f> moveboxVertices; //Vector2f vector of vertices for the movement hitbox
};

#endif
