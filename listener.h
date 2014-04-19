#ifndef LISTENER_H
#define LISTENER_H
#include "gameEvents.h"
namespace GameEvents { class EventQueue; class Event; class DamageEvent; class DamageReceivedEvent;}
class Listener {
public:
    Listener(); 
    virtual ~Listener() {};
    virtual void onDamage(GameEvents::DamageEvent*);
    virtual void onDamageReceived(GameEvents::DamageReceivedEvent*);
    virtual void onMove(GameEvents::Event*);
    virtual void onCollide(GameEvents::Event*);
    virtual void onDeath(GameEvents::Event*);
    virtual void onRoundOver(GameEvents::Event*);

    Listener& operator=(const Listener&);
private:
    virtual void registerListeners()=0;
    GameEvents::EventQueue& eq;
};
#endif
