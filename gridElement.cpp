#include <math.h>
#include "gridElement.h"
#include "gamedata.h"
#include "frameFactory.h"
#include "gameEvents.h"

GridElement::GridElement(const std::string& name) :
  Listener(),
  moveSpeed(Gamedata::getInstance().getXmlFloat(name+"MoveSpeed")),
  gridSprite(name),
  gridPosition(100,100),
  gridVelocity(0,0),
  maxHP(100),
  curHP(100),
  map(MapManager::getInstance()),
  moveDir(),
  moveboxVertices()
{
  //gridSprite.setPosition(map.getOrigin());//+Vector2f(-5,40));//-Vector2f(0,gridSprite.getH())+Vector2f(-5,40));
  gridSprite.setPosition(map.gridToWorld(gridPosition)+Vector2f(-gridSprite.getW()/2,-gridSprite.getH()/2));
  moveDir.reserve(8);
  for(int i=0; i<8; ++i)
    moveDir.push_back(false);

  //fill in the hitbox vertices
  moveboxVertices.reserve(4);
  Vector2f offset(gridSprite.getW() * 0.5, gridSprite.getH() * (2./3.));
  offset += map.getOrigin();

  Vector2f topcorner(gridPosition);
  moveboxVertices.push_back(topcorner);
  moveboxVertices.push_back(topcorner + map.worldToGrid(map.getOrigin() + Vector2f(-gridSprite.getW() * .5,gridSprite.getH() * 0.25)));
  moveboxVertices.push_back(topcorner + map.worldToGrid(map.getOrigin() + Vector2f(gridSprite.getW() * .5,gridSprite.getH() * 0.25)));
  moveboxVertices.push_back(topcorner + map.worldToGrid(map.getOrigin() + Vector2f(0,gridSprite.getH() * .5)));

}

GridElement::GridElement(const GridElement& g) :
  Listener(g),
  moveSpeed(g.moveSpeed),
  gridSprite(g.gridSprite),
  gridPosition(g.gridPosition),
  gridVelocity(g.gridVelocity),
  maxHP(g.maxHP),
  curHP(g.curHP),
  map(MapManager::getInstance()),
  moveDir(g.moveDir),
  moveboxVertices(g.moveboxVertices)
{}

void GridElement::setMoveboxVertex(int indx, Vector2f vert){
 moveboxVertices[indx] = vert;
}

void GridElement::onDamage(int damage) {
    curHP-=damage;
    if(curHP < 0)
    {
        GameEvents::Event d;
        d.type = GameEvents::DEATH_EVENT;
        d.actor = getName();
        d.location = getPosition();
        GameEvents::EventQueue::getInstance().push(d);
        curHP=maxHP;
    }

    // push new damage event
    GameEvents::Event e;
    e.type = GameEvents::DAMAGE_EVENT;
    e.actor = getName();
    e.data.push_back(getHPRatio());
    GameEvents::EventQueue::getInstance().push(e);

}

void GridElement::draw() const {
  gridSprite.draw();
}

void GridElement::update(Uint32 ticks) {

  if (getSprite().getVelocity() != Vector2f(0,0))
    getSprite().update(ticks);

  Vector2f incr = gridVelocity * static_cast<float>(ticks) * 0.001;
  float fticks = static_cast<float>(ticks);

  //recieve validated movement from the map
  Vector2f oldgridPos = gridPosition;
  bool atEdge = false;
  gridPosition = map.validateMovement(*this, incr, fticks, atEdge);

  incr = getSprite().getVelocity() * fticks * 0.001;
  getSprite().setPosition(getSprite().getPosition() + incr);

  Vector2f diff = gridPosition - oldgridPos;
  for(int i=0; i<4; ++i){
    moveboxVertices[i] = moveboxVertices[i] + diff;
  }
  // send off a move event
  if(incr[0] != 0.0 && incr[1] != 0.0) {
    GameEvents::Event e;
    e.type = GameEvents::MOVE_EVENT;
    e.actor = getName();
    e.location = getPosition();
    e.direction = incr;
    GameEvents::EventQueue::getInstance().push(e);
    }
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

void GridElement::registerListeners() {}
