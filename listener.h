#ifndef LISTENER_H
#define LISTENER_H
#include "gameEvents.h"
namespace GameEvents { class EventQueue; }
class Listener {
public:
    Listener(); 
    virtual ~Listener() {};

    Listener& operator=(const Listener&);
private:
    virtual void registerListeners()=0;
    GameEvents::EventQueue& eq;
};
#endif
