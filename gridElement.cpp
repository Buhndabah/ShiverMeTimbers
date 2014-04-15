#include <math.h>
#include "gridElement.h"
#include "gamedata.h"
#include "frameFactory.h"
#include "gameEvents.h"
#include "soundManager.h"

GridElement::GridElement(const std::string& name, int stratNum) :
  Listener(),
  moveSpeed(Gamedata::getInstance().getXmlFloat(name+"MoveSpeed")),
  gridSprite(name),
  gridPosition(95,105),
  gridVelocity(0,0),
  maxHP(100),
  curHP(100),
  map(MapManager::getInstance()),
  moveDir(),
  moveboxVertices(),
  myStrat(NULL)
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



  switch(stratNum) {
      case(CHASE_STRAT):
          myStrat = new ChaseStrategy(this);
          break;
      case(BULLET_STRAT):
          myStrat = new BulletStrategy(this,0);
          break;
      default:
          myStrat = NULL;
          break;
  }
  if(myStrat) 
      myStrat->init();
}

GridElement::GridElement(const std::string& name, const Vector2f& pos, int dir, int stratNum) :
    Listener(),
    moveSpeed(Gamedata::getInstance().getXmlFloat(name+"MoveSpeed")),
    gridSprite(name),
    gridPosition(pos),
    gridVelocity(),
    maxHP(100),
    curHP(100),
    map(MapManager::getInstance()),
    moveDir(),
    moveboxVertices(),
    myStrat(NULL)
{
    gridSprite.setPosition(map.gridToWorld(gridPosition)+Vector2f(-gridSprite.getW()/2,-gridSprite.getH()/2));

    moveDir.reserve(8);
    for(int i=0; i<8; i++)
    {
        moveDir.push_back(false);
    }

    // fill in hitbox vertices
  moveboxVertices.reserve(4);
  Vector2f offset(gridSprite.getW() * 0.5, gridSprite.getH() * (2./3.));
  offset += map.getOrigin();
  Vector2f topcorner(gridPosition);
  moveboxVertices.push_back(topcorner);
  moveboxVertices.push_back(topcorner + map.worldToGrid(map.getOrigin() + Vector2f(-gridSprite.getW() * .5,gridSprite.getH() * 0.25)));
  moveboxVertices.push_back(topcorner + map.worldToGrid(map.getOrigin() + Vector2f(gridSprite.getW() * .5,gridSprite.getH() * 0.25)));
  moveboxVertices.push_back(topcorner + map.worldToGrid(map.getOrigin() + Vector2f(0,gridSprite.getH() * .5)));



  switch(stratNum) {
      case(CHASE_STRAT):
          myStrat = new ChaseStrategy(this);
          break;
      case(BULLET_STRAT):
          myStrat = new BulletStrategy(this,dir);
          break;
      default:
          myStrat = NULL;
          break;
  }
  if(myStrat) 
      myStrat->init();
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
  moveboxVertices(g.moveboxVertices),
  myStrat((g.myStrat->clone()))
{
    if(myStrat) myStrat->init();
}

GridElement& GridElement::operator=(const GridElement& g) {
    if(this == &g) return *this;
    Listener::operator=(g);
    moveSpeed = g.moveSpeed;
    gridSprite = MultiSprite(g.gridSprite);
    gridPosition = g.gridPosition;
    gridVelocity = g.gridVelocity;
    maxHP = g.maxHP;
    curHP = g.curHP;
    moveDir = g.moveDir;
    moveboxVertices = g.moveboxVertices;
    myStrat = g.myStrat->clone();
    return *this;
}

void GridElement::setMoveboxVertex(int indx, Vector2f vert){
 moveboxVertices[indx] = vert;
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
  //if(incr[0] != 0.0 && incr[1] != 0.0) {
    GameEvents::EventQueue::getInstance().push(new GameEvents::MoveEvent(getName(), getPosition(), incr));
    //}
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
  getSprite().velocityX(0.);
  getSprite().velocityY(0.);

  gridVelocityX(0.);
  gridVelocityY(0.);
}

void GridElement::shoot() {
    int i=0;
    for(i; i<moveDir.size(); ++i) { if(moveDir[i]) break; }
    GameEvents::EventQueue::getInstance().push(new GameEvents::CreateEvent(getName(), "snowball", getPosition(), i, BULLET_STRAT));
    SoundManager::getInstance()[1];
}

void GridElement::onDamage(const GameEvents::DamageEvent *e) {
    if(e->getSource().compare(getName()) ==0) return;   // don't respond to ourself
    curHP-=e->getDamage();
    if(curHP < 0)
    {
        GameEvents::EventQueue::getInstance().push(new GameEvents::DeathEvent(getName(), getPosition()));
        curHP=maxHP;
    }

    // push new damage event
    GameEvents::EventQueue::getInstance().push(new GameEvents::DamageEvent(getName(), getPosition(), getHPRatio()));

}

// Forwarding function for damage events
void GridElementDamageForwarder(Listener* context, const GameEvents::Event *e) {
    dynamic_cast<GridElement*>(context)->onDamage(dynamic_cast<const GameEvents::DamageEvent *>(e));
}

void GridElement::registerListeners() { 
    GameEvents::EventQueue::getInstance().addListener(GameEvents::DAMAGE_EVENT, static_cast<Listener*>(this), &GridElementDamageForwarder);
}
