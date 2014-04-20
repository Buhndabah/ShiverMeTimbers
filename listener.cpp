#include "listener.h"

Listener::Listener() :
    eq(GameEvents::EventQueue::getInstance())
{ }

Listener::~Listener() {
    eq.removeListener(this);
}

Listener& Listener::operator=(const Listener& rhs) 
{ 
    if(this == &rhs) return *this;
    return *this;
}

void Listener::onDamage(const GameEvents::DamageEvent *e) {(void)e;}
void Listener::onDamageReceived(const GameEvents::DamageReceivedEvent *e) {(void)e;};
void Listener::onMove(const GameEvents::Event *e) {(void)e;}
void Listener::onCollide(const GameEvents::Event *e) {(void)e;}
void Listener::onDeath(const GameEvents::Event *e) {(void)e;}
void Listener::onRoundOver(const GameEvents::Event *e) {(void)e;}
void Listener::onScore(const GameEvents::ScoreEvent *e) {(void)e;}
