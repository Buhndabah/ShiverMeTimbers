#ifndef GRIDELEMENT__H
#define GRIDELEMENT__H
#include <string>
#include <vector>
#include "vector2f.h"
#include "listener.h"
#include "multisprite.h"
#include "mapManager.h"
#include "strategy.h"

enum dirs {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    UPRIGHT,
    UPLEFT,
    DOWNRIGHT,
    DOWNLEFT
};

class MapManager;

class GridElement : public Listener{
public:
  GridElement(const std::string&, int strat = NONE);
  GridElement(const std::string&, const Vector2f&, int dir, int strat = NONE, Vector2f = Vector2f(0,0));
  GridElement(const GridElement&);
  GridElement& operator=(const GridElement&);
  virtual ~GridElement() { if(myStrat) delete myStrat; }

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
 
  int   getCurrentHP() const { return curHP; }
  void resetHP() { curHP = maxHP; }
  int   getMaxHP() const { return maxHP; }
  float getHPRatio() const { return static_cast<float>(curHP)/static_cast<float>(maxHP); }

  const std::string& getName() const { return name; }
  void setName(std::string& tempName) { name=tempName;}

  std::vector<bool> getMoveDir() const { return moveDir;}

  std::vector<Vector2f> getMoveboxVertices() const { return moveboxVertices;}
  Vector2f getMoveboxVertex(int indx) const { return moveboxVertices[indx];}
  void setMoveboxVertex(int, Vector2f);

  bool getSolid() const { return solid;}
  void setSolid(bool s) { solid = s;}

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

  void moveTowards(Vector2f);
  void shoot();
  void shoot(Vector2f);
  void shoot(dirs);

  virtual void onDamage(const GameEvents::DamageEvent *);

  int getStratType() const { if(myStrat)  return  myStrat->getType(); else return NONE; }
  Strategy* getStrat() const { return myStrat; }

private:
  virtual void registerListeners();
  void clearMoveDir();

  std::string name;
  float moveSpeed;
  MultiSprite gridSprite;
  Vector2f gridPosition;
  Vector2f gridVelocity;
  int maxHP;
  int curHP;
  unsigned int shootTimer;
  const MapManager& map; //the grid the gridElement is attached to
  std::vector<bool> moveDir; //boolean vector telling the gridElement which direction it is moving in
  std::vector<Vector2f> moveboxVertices; //Vector2f vector of vertices for the movement hitbox
  Strategy* myStrat;
  bool solid;
};

void GridElementDamageForwarder(Listener*, const GameEvents::Event*);
#endif
