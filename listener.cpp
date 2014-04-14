#include "listener.h"

Listener::Listener() :
    eq(GameEvents::EventQueue::getInstance())
{ }

Listener& Listener::operator=(const Listener& rhs) 
{ 
    if(this == &rhs) return *this;
    return *this;
}

void Listener::onDamage(GameEvents::DamageEvent *e) {(void)e;}
void Listener::onMove(GameEvents::Event *e) {(void)e;}
void Listener::onCollide(GameEvents::Event *e) {(void)e;}
void Listener::onDeath(GameEvents::Event *e) {(void)e;}
void Listener::onRoundOver(GameEvents::Event *e) {(void)e;}
