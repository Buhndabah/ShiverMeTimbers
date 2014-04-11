#ifndef LISTENER_H
#define LISTENER_H
#include "gameEvents.h"
namespace GameEvents { class EventQueue; struct Event;}
class Listener {
public:
    Listener(); 
    virtual ~Listener() {};
    virtual void onDamage(GameEvents::Event);
    virtual void onMove(GameEvents::Event);
    virtual void onCollide(GameEvents::Event);
    virtual void onDeath(GameEvents::Event);
    virtual void onRoundOver(GameEvents::Event);

    Listener& operator=(const Listener&);
private:
    virtual void registerListeners()=0;
    GameEvents::EventQueue& eq;
};
#endif
