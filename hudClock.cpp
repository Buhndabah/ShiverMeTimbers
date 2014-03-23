#include "hudClock.h"

HUDClock::HUDClock(const Vector2f& p, bool v) : 
    HUDComponent(p, v), 
    io(IOManager::getInstance()),
    clock(Clock::getInstance())
{ }

HUDClock::HUDClock(const HUDClock& rhs) : 
    HUDComponent(rhs),
    io(IOManager::getInstance()),
    clock(Clock::getInstance())
{ }

void HUDClock::draw() const {
    if(isVisible()) {
        io.printMessageValueAt("HUDClock Test: ",clock.getTicks()/1000, getPosition()[0],getPosition()[1]);
    }
}

void HUDClock::update(Uint32 ticks) {
    // nothing
}
