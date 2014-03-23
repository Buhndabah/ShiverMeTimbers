#include <iomanip>
#include <string>
#include <sstream>
#include "hudClock.h"

HUDClock::HUDClock(const Vector2f& p, bool v, int i) :
    HUDComponent(p,v),
    io(IOManager::getInstance()),
    clock(Clock::getInstance()),
    startTime(i),
    countDown(true)
{ }

HUDClock::HUDClock(const Vector2f& p, bool v) : 
    HUDComponent(p, v), 
    io(IOManager::getInstance()),
    clock(Clock::getInstance()),
    startTime(0),
    countDown(false)
{ }

HUDClock::HUDClock(const HUDClock& rhs) : 
    HUDComponent(rhs),
    io(IOManager::getInstance()),
    clock(Clock::getInstance()),
    startTime(rhs.startTime),
    countDown(rhs.countDown)
{ }

void HUDClock::draw() const {
    if(isVisible()) {
        std::stringstream strm;
        std::string time;

        if(countDown) {
            strm << std::setfill('0') << std::setw(2) << (startTime - (clock.getTicks()/1000))/60 << ":"
                 << std::setfill('0') << std::setw(2) << (startTime - (clock.getTicks()/1000))%60 
                 << std::endl;
        }
        else {
            strm << std::setfill('0') << std::setw(2) << clock.getTicks()/1000/60 << ":" 
                << std::setfill('0') << std::setw(2) << clock.getTicks()/1000%60 
                << std::endl;
        }

        strm >> time;
        io.printMessageAt(time, getPosition()[0],getPosition()[1]);
    }
}



void HUDClock::update(Uint32 ticks) {
    // nothing
}
