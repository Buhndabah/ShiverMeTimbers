#include <math.h>
#include <sstream>
#include "clock.h"
#include "gridElement.h"
#include "gamedata.h"
#include "frameFactory.h"
#include "gameEvents.h"
#include "soundManager.h"
#include "mapManager.h"

GridElement::GridElement(const std::string& name, int stratNum) :
  Listener(),
  name(),
  moveSpeed(Gamedata::getInstance().getXmlFloat(name+"MoveSpeed")),
  gridSprite(name),
  gridPosition(95,105),
  gridVelocity(0,0),
  moveDelta(0,0),
  maxHP(100),
  curHP(100),
  shootTimer(0),
  moveDir(),
  moveboxVertices(),
  myStrat(NULL),
  solid(Gamedata::getInstance().getXmlBool(name+"Solid"))
{
  
  Vector2f mapOriginAsWorld = MapManager::getInstance().getOrigin();

  // initially set all move states to false
  moveDir.reserve(8);
  for(int i=0; i<8; ++i) {
    moveDir.push_back(false);
  }

  // Grab position from map
  gridSprite.setPosition(gridPosition.fromIso(mapOriginAsWorld)+Vector2f(-gridSprite.getW()/2,-gridSprite.getH()/2));

  //fill in the movebox vertices
  moveboxVertices.reserve(4);
  Vector2f offset(gridSprite.getW() * 0.5, gridSprite.getH() * (2./3.));
  offset += mapOriginAsWorld;

  Vector2f topcorner(gridPosition);
  moveboxVertices.push_back(topcorner); // middle top
  moveboxVertices.push_back(topcorner + (mapOriginAsWorld + Vector2f(-gridSprite.getW() * .5,gridSprite.getH() * .25)).toIso(mapOriginAsWorld)); // left
  moveboxVertices.push_back(topcorner + (mapOriginAsWorld + Vector2f(gridSprite.getW() * .5,gridSprite.getH() * .25)).toIso(mapOriginAsWorld)); // right
  moveboxVertices.push_back(topcorner + (mapOriginAsWorld + Vector2f(0,gridSprite.getH() *.5)).toIso(mapOriginAsWorld)); // bottom

  // Assign ourself an ID from MapManager::getInstance().
  // XXX We shouldn't be referencing the MapManager::getInstance().for this. It should either be passed in or a static var
  std::ostringstream sstream;
  sstream << MapManager::getInstance().getNumGridElements();
  std::string tempname = std::string(gridSprite.getName()) + sstream.str();
  setName(tempname);

  // Assign a strategy
  switch(stratNum) {
      case(CHASE_STRAT):
          myStrat = new ChaseStrategy(this,MapManager::getInstance().getPlayer());
          break;
      case(BULLET_STRAT):
          tempname = tempname + std::string("bullet");
          myStrat = new BulletStrategy(this,0);
          break;
      case(TURRET_STRAT):
          myStrat = new TurretStrategy(this, MapManager::getInstance().getPlayer());
          break;
      default:
          myStrat = NULL;
          break;
  }
  // initialize the strategy
  if(myStrat) {
      myStrat->init();
  }

  // register with eventQueue
  registerListeners();
}

GridElement::GridElement(const std::string& name, const Vector2f& pos, int dir, int stratNum, Vector2f target) :
    Listener(),
    name(),
    moveSpeed(Gamedata::getInstance().getXmlFloat(name+"MoveSpeed")),
    gridSprite(name),
    gridPosition(pos),
    gridVelocity(),
    moveDelta(0,0),
    maxHP(100),
    curHP(100),
    shootTimer(0),
    moveDir(),
    moveboxVertices(),
    myStrat(NULL),
    solid(Gamedata::getInstance().getXmlBool(name+"Solid"))
{

    Vector2f mapOriginAsWorld = MapManager::getInstance().getOrigin();

    // initially we are not moving
    moveDir.reserve(8);
    for(int i=0; i<8; i++)
    {
        moveDir.push_back(false);
    }

    // set position
    gridSprite.setPosition(gridPosition.fromIso(mapOriginAsWorld)+Vector2f(-gridSprite.getW()/2,-gridSprite.getH()/2));

    // fill in hitbox vertices
  moveboxVertices.reserve(4);
  Vector2f offset(gridSprite.getW() * 0.5, gridSprite.getH() * (2./3.));
  offset += mapOriginAsWorld;
  Vector2f topcorner(gridPosition);
  moveboxVertices.push_back(topcorner);
  moveboxVertices.push_back(topcorner + (mapOriginAsWorld + Vector2f(-gridSprite.getW() * .5,gridSprite.getH() * .25)).toIso(mapOriginAsWorld));
  moveboxVertices.push_back(topcorner + (mapOriginAsWorld + Vector2f(gridSprite.getW() * .5,gridSprite.getH() * .25)).toIso(mapOriginAsWorld));
  moveboxVertices.push_back(topcorner + (mapOriginAsWorld + Vector2f(0,gridSprite.getH() * .5)).toIso(mapOriginAsWorld));

  // Assign self id
  std::ostringstream sstream;
  sstream << MapManager::getInstance().getNumGridElements();
  std::string tempname = std::string(gridSprite.getName()) + sstream.str();
  setName(tempname);

  // Assign strategy
  switch(stratNum) {
      case(CHASE_STRAT):
          myStrat = new ChaseStrategy(this,MapManager::getInstance().getPlayer());
          break;
      case(BULLET_STRAT):
          tempname = tempname + std::string("bullet");
	  if(dir == -1)
              myStrat = new BulletStrategy(this,target);
          else
	      myStrat = new BulletStrategy(this,dir);
          break;
      case(TURRET_STRAT):
          myStrat = new TurretStrategy(this, MapManager::getInstance().getPlayer());
          break;
      default:
          myStrat = NULL;
          break;
  }

  // Init strategy
  if(myStrat) {
      myStrat->init();
  }

  // register with eventQueue
  registerListeners();
}

GridElement::GridElement(const GridElement& g) :
  Listener(g),
  name(),
  moveSpeed(g.moveSpeed),
  gridSprite(g.gridSprite),
  gridPosition(g.gridPosition),
  gridVelocity(g.gridVelocity),
  moveDelta(0,0),
  maxHP(g.maxHP),
  curHP(g.curHP),
  shootTimer(0),
  moveDir(g.moveDir),
  moveboxVertices(g.moveboxVertices),
  myStrat((g.myStrat->clone())),
  solid(g.solid)
{
  std::ostringstream sstream;
  sstream << MapManager::getInstance().getNumGridElements();
  std::string tempname = std::string(gridSprite.getName()) + sstream.str();

  // mark self as bullet
  if(myStrat != NULL && myStrat->getType()==BULLET_STRAT) {
    tempname = tempname + std::string("bullet");
  }
  setName(tempname);

  // init strat
  if(myStrat) {
    myStrat->init();
  }

  // register with eventQueue
  registerListeners();
}

GridElement& GridElement::operator=(const GridElement& g) {
    if(this == &g) return *this;
    Listener::operator=(g);
    moveSpeed = g.moveSpeed;
    gridSprite = MultiSprite(g.gridSprite);
    gridPosition = g.gridPosition;
    gridVelocity = g.gridVelocity;
    moveDelta = Vector2f(0,0);
    maxHP = g.maxHP;
    curHP = g.curHP;
    moveDir = g.moveDir;
    moveboxVertices = g.moveboxVertices;
    myStrat = g.myStrat->clone();
    solid = g.solid;

  // name ourself
  std::ostringstream sstream;
  sstream << MapManager::getInstance().getNumGridElements();
  std::string tempname = std::string(gridSprite.getName()) + sstream.str();
    if(myStrat->getType()==BULLET_STRAT)
    {
        tempname= tempname+std::string("bullet");
    }
  setName(tempname);

  if(myStrat) {
    myStrat->init();
  }

  registerListeners();

  return *this;
}

void GridElement::setMoveboxVertex(int indx, Vector2f vert){
    moveboxVertices[indx] = vert;
}

// draw our sprite
void GridElement::draw() const {
    gridSprite.draw();
}


void GridElement::update(Uint32 ticks) {

  // if we're moving
  if (getSprite().getVelocity() != Vector2f(0,0)) {
    getSprite().update(ticks);
  }

  // calculate how far we're moving
  float fticks = static_cast<float>(ticks);
  moveDelta = gridVelocity * fticks * 0.001;
}

// Set our pos to pos + delta
void GridElement::applyMoveDelta() {

  // set our position to that
  for(int i=0; i<4; ++i){
    moveboxVertices[i] = moveboxVertices[i] + moveDelta;
  }

  // update our gridPosition
  gridPosition += moveDelta;

  // set gridSprite position to that
  gridSprite.setPosition(gridPosition.fromIso(MapManager::getInstance().getOrigin())+Vector2f(-gridSprite.getW()/2,-gridSprite.getH()/2));

  // fire off a move event
  GameEvents::EventQueue::getInstance().push(new GameEvents::MoveEvent(getName(), getPosition(), moveDelta));

  // reset move
  moveDelta = (Vector2f(0,0));

}

// reset movement flags
void GridElement::clearMoveDir() {
  for(int i=0; i<8; ++i)
    moveDir[i] = false;
}

void GridElement::moveUp() {
  clearMoveDir();
  moveDir[UP] = true;
  getSprite().velocityX(0.);
  getSprite().velocityY(-moveSpeed * (cos(26.565*M_PI/180) / cos(45*M_PI/180) * 0.5));

  gridVelocityX(-moveSpeed * cos(45*M_PI/180));
  gridVelocityY(gridVelocityX());
}

void GridElement::moveUpLeft() {
  clearMoveDir();
  moveDir[UPLEFT] = true;

  getSprite().velocityX(-moveSpeed * cos(26.565*M_PI/180));
  getSprite().velocityY(-moveSpeed * sin(26.565*M_PI/180));

  gridVelocityX(0.);
  gridVelocityY(-moveSpeed);
}

void GridElement::moveUpRight() {
  clearMoveDir();
  moveDir[UPRIGHT] = true;

  getSprite().velocityX(moveSpeed * cos(26.565*M_PI/180));
  getSprite().velocityY(-moveSpeed * sin(26.565*M_PI/180));

  gridVelocityX(-moveSpeed);
  gridVelocityY(0.);
}

void GridElement::moveDown() {
  clearMoveDir();
  moveDir[DOWN] = true;

  getSprite().velocityX(0.);
  //getSprite().velocityY(moveSpeed/sqrt(2));
  getSprite().velocityY(moveSpeed * (cos(26.565*M_PI/180) / cos(45*M_PI/180) * 0.5));

  gridVelocityX(moveSpeed * cos(45*M_PI/180));
  gridVelocityY(gridVelocityX());
}

void GridElement::moveDownLeft() {
  clearMoveDir();
  moveDir[DOWNLEFT] = true;

  getSprite().velocityX(-moveSpeed * cos(26.565*M_PI/180));
  getSprite().velocityY(moveSpeed * sin(26.565*M_PI/180));

  gridVelocityX(moveSpeed);
  gridVelocityY(0.);
}

void GridElement::moveDownRight() {
  clearMoveDir();
  moveDir[DOWNRIGHT] = true;

  getSprite().velocityX(moveSpeed * cos(26.565*M_PI/180));
  getSprite().velocityY(moveSpeed * sin(26.565*M_PI/180));

  gridVelocityX(0.);
  gridVelocityY(moveSpeed);
}

void GridElement::moveLeft() {
  clearMoveDir();
  moveDir[LEFT] = true;

  getSprite().velocityX(-moveSpeed *(cos(26.565*M_PI/180) / cos(45*M_PI/180)));
  getSprite().velocityY(0.);

  gridVelocityX(moveSpeed * cos(45*M_PI/180));
  gridVelocityY(-gridVelocityX());
}

void GridElement::moveRight() {
  clearMoveDir();
  moveDir[RIGHT] = true;

  getSprite().velocityX(moveSpeed *(cos(26.565*M_PI/180) / cos(45*M_PI/180)));
  getSprite().velocityY(0.);

  gridVelocityX(-moveSpeed * cos(45*M_PI/180));
  gridVelocityY(-gridVelocityX());
}

void GridElement::stop() {
  getSprite().velocityX(0.);
  getSprite().velocityY(0.);

  gridVelocityX(0.);
  gridVelocityY(0.);
}

// run at position
void GridElement::moveTowards(Vector2f target){
  Vector2f worldDir = target - gridPosition.fromIso(MapManager::getInstance().getOrigin());
  Vector2f gridDir = target.toIso(MapManager::getInstance().getOrigin()) - gridPosition;
  worldDir = worldDir.normalize();
  gridDir = gridDir.normalize();

  getSprite().velocityX(1);
  getSprite().velocityY(1);

  gridVelocityX(moveSpeed * gridDir[0]);
  gridVelocityY(moveSpeed * gridDir[1]); 

}


void GridElement::shoot() {
    
    unsigned int curTicks = Clock::getInstance().getTotalTicks();

    // are we ready to shoot again?
    if(curTicks - shootTimer > 1000 ) {

        shootTimer = curTicks;
     
        // figure out what direction to shoot
        int i=0;
        for(i; i<moveDir.size(); ++i) { if(moveDir[i]) break; }

        GameEvents::EventQueue::getInstance().push(new GameEvents::CreateEvent(getName(), "snowball", gridPosition.fromIso(MapManager::getInstance().getOrigin()), i, Vector2f(0,0), BULLET_STRAT));
        SoundManager::getInstance()[1];
    }
}

void GridElement::shoot(Vector2f target) {

    unsigned int curTicks = Clock::getInstance().getTotalTicks();
    // Are we ready to shoot?
    if(curTicks - shootTimer > 1000) {

        shootTimer = curTicks;

        GameEvents::EventQueue::getInstance().push(new GameEvents::CreateEvent(getName(), "snowball", gridPosition.fromIso(MapManager::getInstance().getOrigin()), -1, target, BULLET_STRAT));
        SoundManager::getInstance()[1];
    }
}


void GridElement::shoot(dirs dir) {

    unsigned int curTicks = Clock::getInstance().getTotalTicks();
    if(curTicks - shootTimer > 1000) {

        shootTimer = curTicks;

        GameEvents::EventQueue::getInstance().push(new GameEvents::CreateEvent(getName(), "snowball", getPosition() + gridSprite.getSize()/2, dir, Vector2f(0,0), BULLET_STRAT));

    }
}

/*********** Stuff for handling events ***************/

void GridElement::onDamage(const GameEvents::DamageEvent *e) {

    // Check if event is targeting self
    if(e->getTarget().compare(getName()) !=0) {  return; }

    curHP-=e->getDamage();

    // What to do if we die
    if(curHP <= 0)
    {
        // Push a death notification
        GameEvents::EventQueue::getInstance().push(new GameEvents::DeathEvent(getName(), getPosition()));
        curHP=maxHP;
    }

    // Push a damage notification
    GameEvents::EventQueue::getInstance().push(new GameEvents::DamageReceivedEvent(getName(), getPosition(), getHPRatio()));

}

// Forwarding function for damage events
void GridElementDamageForwarder(Listener* context, const GameEvents::Event *e) {
    dynamic_cast<GridElement*>(context)->onDamage(dynamic_cast<const GameEvents::DamageEvent *>(e));
}

void GridElement::registerListeners() { 
    GameEvents::EventQueue::getInstance().addListener(GameEvents::DAMAGE_EVENT, static_cast<Listener*>(this), &GridElementDamageForwarder);
}
