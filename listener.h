#ifndef LISTENER_H
#define LISTENER_H
#include "gameEvents.h"
namespace GameEvents { 
        class EventQueue; 
        class Event; 
        class DamageEvent; 
        class DamageReceivedEvent;
        class ScoreEvent;
}

class Listener {
public:
    Listener(); 
    virtual ~Listener(); 
    virtual void onDamage(const GameEvents::DamageEvent*);
    virtual void onDamageReceived(const GameEvents::DamageReceivedEvent*);
    virtual void onMove(const GameEvents::Event*);
    virtual void onCollide(const GameEvents::Event*);
    virtual void onDeath(const GameEvents::Event*);
    virtual void onHUDRemove(const GameEvents::Event*);
    virtual void onRoundOver(const GameEvents::Event*);
    virtual void onScore(const GameEvents::ScoreEvent*);

    Listener& operator=(const Listener&);
private:
    virtual void registerListeners()=0;
    GameEvents::EventQueue& eq;
};
#endif
