#include <math.h>
#include "gridElement.h"
#include "gamedata.h"
#include "frameFactory.h"

GridElement::GridElement(const std::string& name) :
  moveSpeed(Gamedata::getInstance().getXmlFloat(name+"MoveSpeed")),
  gridSprite(name),
  gridPosition(0,0),
  gridVelocity(0,0),
  dummy(DummyGrid::getInstance()),
  moveDir()
{
  moveDir.reserve(8);
  for(int i=0; i<8; ++i)
    moveDir.push_back(false);
}

GridElement::GridElement(const GridElement& g) :
  moveSpeed(g.moveSpeed),
  gridSprite(g.gridSprite),
  gridPosition(g.gridPosition),
  gridVelocity(g.gridVelocity),
  dummy(DummyGrid::getInstance()),
  moveDir(g.moveDir)
{}

void GridElement::update(Uint32 ticks) {

//  if (getSprite().getVelocity() != Vector2f(0,0))
  //  getSprite().advanceFrame(ticks);

//  getSprite().velocityX(gridVelocityX() - gridVelocityY());
//  getSprite().velocityY((gridVelocityX() + gridVelocityY()) / 2);


  Vector2f incr = gridVelocity * static_cast<float>(ticks) * 0.001;
  Vector2f oldGridPos;// = gridPosition;
  Vector2f newGridPos;// = gridPosition;
  float dist;
  bool atEdge = false;

  //Check right X border
std::cout << "HURRBADURRBA " << gridPosition[0] + incr[0] << std::endl << std::endl;
  if((gridPosition[0] + incr[0] > dummy.getW() * dummy.getTileWidth())){
    oldGridPos = gridPosition;
    gridPosition[0] = dummy.getW() * dummy.getTileWidth();
    dist = gridPosition[0] - oldGridPos[0];

    if(moveDir[0]) //if moving up
      gridPosition[1] = gridPosition[1] - dist;
    if(moveDir[7]) //if moving right
      gridPosition[1] = gridPosition[1] + dist;

    dist = sqrt( pow(gridPosition[0] - oldGridPos[0],2) + pow(gridPosition[1] - oldGridPos[1],2));
    ticks = dist / static_cast<float>(moveSpeed);
    atEdge = true;
  }
  //Check left X border
  else if(gridPosition[0] + incr[0] < 0){
    oldGridPos = gridPosition;
    gridPosition[0] = 0;
    dist = -oldGridPos[0];

    if(moveDir[3]) //if moving down
      gridPosition[1] = gridPosition[1] - dist;
    if(moveDir[6]) //if moving left
      gridPosition[1] = gridPosition[1] + dist;

    dist = sqrt( pow(gridPosition[0] - oldGridPos[0],2) + pow(gridPosition[1] - oldGridPos[1],2));
    ticks = dist / static_cast<float>(moveSpeed);
    atEdge = true;
 
  }
  //Check bottom Y border
  else if(gridPosition[1] + incr[1] > dummy.getH() * dummy.getTileWidth()){
    oldGridPos = gridPosition;
    gridPosition[1] = dummy.getH() * dummy.getTileWidth();
    dist = gridPosition[1] - oldGridPos[1];

    if(moveDir[3]) //if moving down
      gridPosition[0] = gridPosition[0] - dist; 
    if(moveDir[7]) //if moving right
      gridPosition[0] = gridPosition[0] + dist; 

    dist = sqrt( pow(gridPosition[0] - oldGridPos[0],2) + pow(gridPosition[1] - oldGridPos[1],2));
    ticks = dist/ static_cast<float>(moveSpeed);
    atEdge = true;
  }
  //Check top Y border
  else if(gridPosition[1] + incr[1] < 0){
    oldGridPos = gridPosition;
    gridPosition[1] = 0;
    dist = -oldGridPos[1];

    if(moveDir[0]) //if moving up
      gridPosition[0] = gridPosition[0] - dist; 
    if(moveDir[6]) //if moving left
      gridPosition[0] = gridPosition[0] + dist; 

    dist = sqrt( pow(gridPosition[0] - oldGridPos[0],2) + pow(gridPosition[1] - oldGridPos[1],2));
    ticks = dist/ static_cast<float>(moveSpeed);
    atEdge = true;
 
  }
  else
    gridPosition += incr;

  incr = getSprite().getVelocity() * static_cast<float>(ticks) * 0.001;
  getSprite().setPosition(getSprite().getPosition() + incr);

  if(atEdge)
    stop();
//std::cout << "velocity: " << getSprite().getVelocity() << std::endl;
//std::cout << "\t" << getSprite().getPosition() << std::endl << std::endl;
}

void GridElement::clearMoveDir() {
  for(int i=0; i<8; ++i)
    moveDir[i] = false;
}

void GridElement::moveUp() {
  clearMoveDir();
  moveDir[0] = true;
  getSprite().velocityX(0.);
  getSprite().velocityY(-moveSpeed/sqrt(2));

  gridVelocityX(moveSpeed * cos(45*3.141592653589/180));
  gridVelocityY(-gridVelocityX());
}

void GridElement::moveUpLeft() {
  clearMoveDir();
  moveDir[1] = true;

  getSprite().velocityX(-moveSpeed * cos(30*3.141592653589/180));
  getSprite().velocityY(-moveSpeed * sin(30*3.141592653589/180));

  gridVelocityX(0.);
  gridVelocityY(-moveSpeed);
}

void GridElement::moveUpRight() {
  clearMoveDir();
  moveDir[2] = true;

  getSprite().velocityX(moveSpeed * cos(30*3.141592653589/180));
  getSprite().velocityY(-moveSpeed * sin(30*3.141592653589/180));

  gridVelocityX(moveSpeed);
  gridVelocityY(0.);
}

void GridElement::moveDown() {
  clearMoveDir();
  moveDir[3] = true;

  getSprite().velocityX(0.);
  getSprite().velocityY(moveSpeed/sqrt(2));

  gridVelocityX(-moveSpeed * cos(45*3.141592653589/180));
  gridVelocityY(-gridVelocityX());
}

void GridElement::moveDownLeft() {
  clearMoveDir();
  moveDir[4] = true;

  getSprite().velocityX(-moveSpeed * cos(30*3.141592653589/180));
  getSprite().velocityY(moveSpeed * sin(30*3.141592653589/180));

  gridVelocityX(-moveSpeed);
  gridVelocityY(0.);
}

void GridElement::moveDownRight() {
  clearMoveDir();
  moveDir[5] = true;

  getSprite().velocityX(moveSpeed * cos(30*3.141592653589/180));
  getSprite().velocityY(moveSpeed * sin(30*3.141592653589/180));

  gridVelocityX(0.);
  gridVelocityY(moveSpeed);
}

void GridElement::moveLeft() {
  clearMoveDir();
  moveDir[6] = true;

  getSprite().velocityX(-moveSpeed / (sqrt(2)/sqrt(3)));
  getSprite().velocityY(0.);

  gridVelocityX(-moveSpeed * cos(45*3.141592653589/180));
  gridVelocityY(gridVelocityX());
}

void GridElement::moveRight() {
  clearMoveDir();
  moveDir[7] = true;

  getSprite().velocityX(moveSpeed / (sqrt(2)/sqrt(3)));
  getSprite().velocityY(0.);

  gridVelocityX(moveSpeed * cos(45*3.141592653589/180));
  gridVelocityY(gridVelocityX());
}

void GridElement::stop() {
  clearMoveDir();
  getSprite().velocityX(0.);
  getSprite().velocityY(0.);

  gridVelocityX(0.);
  gridVelocityY(0.);
}

