#include <math.h>
#include "gridElement.h"
#include "gamedata.h"
#include "frameFactory.h"

GridElement::GridElement(const std::string& name) :
  moveSpeed(Gamedata::getInstance().getXmlFloat(name+"MoveSpeed")),
  gridSprite(name),
  gridPosition(0,0),
  gridVelocity(0,0),
  maxHP(100),
  curHP(100),
  map(MapManager::getInstance()),
  moveDir()
{
    std::cerr<< "Map size: " << map.getW() << " " << map.getH() << std::endl;
    std::cerr<< "Tile size: " << map.getTileWidth() << " " << map.getTileHeight() << std::endl;
  // numbers below obtained through science
  gridSprite.setPosition(map.getOrigin()-Vector2f(0,gridSprite.getH())+Vector2f(-5,40));
  moveDir.reserve(8);
  for(int i=0; i<8; ++i)
    moveDir.push_back(false);
}

GridElement::GridElement(const GridElement& g) :
  moveSpeed(g.moveSpeed),
  gridSprite(g.gridSprite),
  gridPosition(g.gridPosition),
  gridVelocity(g.gridVelocity),
  maxHP(g.maxHP),
  curHP(g.curHP),
  map(MapManager::getInstance()),
  moveDir(g.moveDir)
{}

void GridElement::onDamage(int damage) {
    curHP-=damage;
    if(curHP < 0)
    {
        curHP=maxHP;
    }

}

void GridElement::update(Uint32 ticks) {

  if (getSprite().getVelocity() != Vector2f(0,0))
    getSprite().update(ticks);

  Vector2f incr = gridVelocity * static_cast<float>(ticks) * 0.001;
  float fticks = static_cast<float>(ticks);

  int moveDirIndx = 0;
  for(int i=0; i<8; ++i)
    if(moveDir[i]) moveDirIndx = i;

  bool atEdge = false;
  //gridPosition = map.validateMovement(gridPosition, gridPosition + incr, moveDirIndx, fticks, moveSpeed, atEdge);
  gridPosition = map.validateMovement(*this, gridPosition + incr, fticks, atEdge);

  incr = getSprite().getVelocity() * fticks * 0.001;
  getSprite().setPosition(getSprite().getPosition() + incr);

  if(atEdge)
    stop();
}

void GridElement::clearMoveDir() {
  for(int i=0; i<8; ++i)
    moveDir[i] = false;
}

void GridElement::moveUp() {
  clearMoveDir();
  moveDir[0] = true;
  getSprite().velocityX(0.);
  getSprite().velocityY(-moveSpeed * (cos(26.565*3.141592653589/180) / cos(45*3.141592653589/180) * 0.5));

  gridVelocityX(-moveSpeed * cos(45*3.141592653589/180));
  gridVelocityY(gridVelocityX());
}

void GridElement::moveUpLeft() {
  clearMoveDir();
  moveDir[1] = true;

  getSprite().velocityX(-moveSpeed * cos(26.565*3.141592653589/180));
  getSprite().velocityY(-moveSpeed * sin(26.565*3.141592653589/180));

  gridVelocityX(0.);
  gridVelocityY(-moveSpeed);
}

void GridElement::moveUpRight() {
  clearMoveDir();
  moveDir[2] = true;

  getSprite().velocityX(moveSpeed * cos(26.565*3.141592653589/180));
  getSprite().velocityY(-moveSpeed * sin(26.565*3.141592653589/180));

  gridVelocityX(-moveSpeed);
  gridVelocityY(0.);
}

void GridElement::moveDown() {
  clearMoveDir();
  moveDir[3] = true;

  getSprite().velocityX(0.);
  //getSprite().velocityY(moveSpeed/sqrt(2));
  getSprite().velocityY(moveSpeed * (cos(26.565*3.141592653589/180) / cos(45*3.141592653589/180) * 0.5));

  gridVelocityX(moveSpeed * cos(45*3.141592653589/180));
  gridVelocityY(gridVelocityX());
}

void GridElement::moveDownLeft() {
  clearMoveDir();
  moveDir[4] = true;

  getSprite().velocityX(-moveSpeed * cos(26.565*3.141592653589/180));
  getSprite().velocityY(moveSpeed * sin(26.565*3.141592653589/180));

  gridVelocityX(moveSpeed);
  gridVelocityY(0.);
}

void GridElement::moveDownRight() {
  clearMoveDir();
  moveDir[5] = true;

  getSprite().velocityX(moveSpeed * cos(26.565*3.141592653589/180));
  getSprite().velocityY(moveSpeed * sin(26.565*3.141592653589/180));

  gridVelocityX(0.);
  gridVelocityY(moveSpeed);
}

void GridElement::moveLeft() {
  clearMoveDir();
  moveDir[6] = true;

  getSprite().velocityX(-moveSpeed *(cos(26.565*3.141592653589/180) / cos(45*3.141592653589/180)));
  getSprite().velocityY(0.);

  gridVelocityX(moveSpeed * cos(45*3.141592653589/180));
  gridVelocityY(-gridVelocityX());
}

void GridElement::moveRight() {
  clearMoveDir();
  moveDir[7] = true;

  getSprite().velocityX(moveSpeed *(cos(26.565*3.141592653589/180) / cos(45*3.141592653589/180)));
  getSprite().velocityY(0.);

  gridVelocityX(-moveSpeed * cos(45*3.141592653589/180));
  gridVelocityY(-gridVelocityX());
}

void GridElement::stop() {
  clearMoveDir();
  getSprite().velocityX(0.);
  getSprite().velocityY(0.);

  gridVelocityX(0.);
  gridVelocityY(0.);
}

