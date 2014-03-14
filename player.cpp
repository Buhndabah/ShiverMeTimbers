#include <string>
#include <math.h>
#include "player.h"
#include "gamedata.h"
#include "frameFactory.h"

Player::Player(const std::string& name) :
  MultiSprite(name),
  gridPosition(0,0),
  gridVelocity(0,0),
  moveSpeed(Gamedata::getInstance().getXmlFloat(name+"MoveSpeed"))
{}

Player::Player(const Player& p) :
  MultiSprite(p),
  gridPosition(p.gridPosition),
  gridVelocity(p.gridVelocity),
  moveSpeed(p.moveSpeed)
{
}

/*void Player::draw() const {
  MultiSprite::draw();
}*/

void Player::update(Uint32 ticks) {
  //MultiSprite::update(ticks);

 // if (getVelocity() != Vector2f(0,0))
   // advanceFrame(ticks);

//  velocityX(gridVelocityX() - gridVelocityY());
//  velocityY((gridVelocityX() + gridVelocityY()) / 2);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

//std::cout << "velocity: " << getVelocity() << std::endl;
//std::cout << "\t" << getPosition() << std::endl << std::endl;
}
void Player::moveUp() {
  velocityX(0.);
  velocityY(-moveSpeed/sqrt(2));
}

void Player::moveUpLeft() {
  velocityX(-moveSpeed * cos(30*3.141596265/180));
  velocityY(-moveSpeed * sin(30*3.141596265/180));
}

void Player::moveUpRight() {
  velocityX(moveSpeed * cos(30*3.141596265/180));
  velocityY(-moveSpeed * sin(30*3.141596265/180));
}

void Player::moveDown() {
  velocityX(0.);
  velocityY(moveSpeed/sqrt(2));
}

void Player::moveDownLeft() {
  velocityX(-moveSpeed * cos(30*3.141596265/180));
  velocityY(moveSpeed * sin(30*3.141596265/180));
}

void Player::moveDownRight() {
  velocityX(moveSpeed * cos(30*3.141596265/180));
  velocityY(moveSpeed * sin(30*3.141596265/180));
}
void Player::moveLeft() {
  velocityX(-moveSpeed / (sqrt(2)/sqrt(3)));
  velocityY(0.);
}

void Player::moveRight() {
  velocityX(moveSpeed / (sqrt(2)/sqrt(3)));
  velocityY(0.);
}

void Player::stop() {
  velocityX(0.);
  velocityY(0.);
}
