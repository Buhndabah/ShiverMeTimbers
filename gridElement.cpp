#include <math.h>
#include "gridElement.h"
#include "gamedata.h"
#include "frameFactory.h"
GridElement::GridElement(const std::string& name) :
  moveSpeed(Gamedata::getInstance().getXmlFloat(name+"MoveSpeed")),
  gridSprite(name),
  gridPosition(0,0),
  gridVelocity(0,0)
{}

GridElement::GridElement(const GridElement& g) :
  moveSpeed(g.moveSpeed),
  gridSprite(g.gridSprite),
  gridPosition(g.gridPosition),
  gridVelocity(g.gridVelocity)
{}

void GridElement::update(Uint32 ticks) {

//  if (getSprite().getVelocity() != Vector2f(0,0))
  //  getSprite().advanceFrame(ticks);

//  getSprite().velocityX(gridVelocityX() - gridVelocityY());
//  getSprite().velocityY((gridVelocityX() + gridVelocityY()) / 2);

  Vector2f incr = getSprite().getVelocity() * static_cast<float>(ticks) * 0.001;
  getSprite().setPosition(getSprite().getPosition() + incr);

//std::cout << "velocity: " << getSprite().getVelocity() << std::endl;
//std::cout << "\t" << getSprite().getPosition() << std::endl << std::endl;
}

void GridElement::moveUp() {
  getSprite().velocityX(0.);
  getSprite().velocityY(-moveSpeed/sqrt(2));
}

void GridElement::moveUpLeft() {
  getSprite().velocityX(-moveSpeed * cos(30*3.141596265/180));
  getSprite().velocityY(-moveSpeed * sin(30*3.141596265/180));
}

void GridElement::moveUpRight() {
  getSprite().velocityX(moveSpeed * cos(30*3.141596265/180));
  getSprite().velocityY(-moveSpeed * sin(30*3.141596265/180));
}

void GridElement::moveDown() {
  getSprite().velocityX(0.);
  getSprite().velocityY(moveSpeed/sqrt(2));
}

void GridElement::moveDownLeft() {
  getSprite().velocityX(-moveSpeed * cos(30*3.141596265/180));
  getSprite().velocityY(moveSpeed * sin(30*3.141596265/180));
}

void GridElement::moveDownRight() {
  getSprite().velocityX(moveSpeed * cos(30*3.141596265/180));
  getSprite().velocityY(moveSpeed * sin(30*3.141596265/180));
}
void GridElement::moveLeft() {
  getSprite().velocityX(-moveSpeed / (sqrt(2)/sqrt(3)));
  getSprite().velocityY(0.);
}

void GridElement::moveRight() {
  getSprite().velocityX(moveSpeed / (sqrt(2)/sqrt(3)));
  getSprite().velocityY(0.);
}

void GridElement::stop() {
  getSprite().velocityX(0.);
  getSprite().velocityY(0.);
}

