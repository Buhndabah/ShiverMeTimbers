#include <iomanip>
#include <string>
#include <sstream>
#include "hudClock.h"

HUDClock::HUDClock(const std::string& n, const Vector2f& p, bool v, int i) :
    HUDComponent(n,p,v),
    io(IOManager::getInstance()),
    clock(Clock::getInstance()),
    startTime(i),
    countDown(true)
{ }

HUDClock::HUDClock(const std::string& n, const Vector2f& p, bool v) : 
    HUDComponent(n,p,v), 
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

HUDClock& HUDClock::operator=(const HUDClock& rhs) 
{
    HUDComponent::operator=(rhs);
    startTime = rhs.startTime;
    countDown = rhs.countDown;
    return *this;
}

void HUDClock::draw() const {
    if(isVisible()) {
        std::stringstream strm;
        std::string time;

        if(countDown && ( (startTime-(clock.getTicks()/1000)) >0)) {
            strm << std::setfill('0') << std::setw(2) << (startTime - (clock.getTicks()/1000))/60 << ":"
                 << std::setfill('0') << std::setw(2) << (startTime - (clock.getTicks()/1000))%60 
                 << std::endl;
            strm >> time;
            io.printMessageAt(time, getPosition()[0],getPosition()[1]);
        }
        else if(!countDown){
            strm << std::setfill('0') << std::setw(2) << clock.getTicks()/1000/60 << ":" 
                << std::setfill('0') << std::setw(2) << clock.getTicks()/1000%60 
                << std::endl;
            strm >> time;
            io.printMessageAt(time, getPosition()[0],getPosition()[1]);
        }

    }
}



void HUDClock::update(Uint32 ticks) {
    // nothing
}
