#include <iomanip>
#include <string>
#include <sstream>
#include "hudClock.h"

HUDClock::HUDClock(const std::string& n, const Vector2f& p, bool v, int i) :
    HUDComponent(n,p,v),
    io(IOManager::getInstance()),
    clock(Clock::getInstance()),
    totalTime(0),
    time(" "),
    startTime(i),
    countDown(true),
    stop(false)
{ }

HUDClock::HUDClock(const std::string& n, const Vector2f& p, bool v) : 
    HUDComponent(n,p,v), 
    io(IOManager::getInstance()),
    clock(Clock::getInstance()),
    totalTime(0),
    time(" "),
    startTime(0),
    countDown(false),
    stop(false)
{ }

HUDClock::HUDClock(const HUDClock& rhs) : 
    HUDComponent(rhs),
    io(IOManager::getInstance()),
    clock(Clock::getInstance()),
    totalTime(rhs.totalTime),
    time(rhs.time),
    startTime(rhs.startTime),
    countDown(rhs.countDown),
    stop(rhs.stop)
{ }

HUDClock& HUDClock::operator=(const HUDClock& rhs) 
{
    HUDComponent::operator=(rhs);
    totalTime = rhs.totalTime;
    time = rhs.time;
    startTime = rhs.startTime;
    countDown = rhs.countDown;
    stop = rhs.stop;
    return *this;
}

void HUDClock::draw() const {
    if(isVisible()) 
    {
        io.printMessageAt(time, getPosition()[0],getPosition()[1]);
    }
}



void HUDClock::update(Uint32 ticks) {
    std::stringstream strm;
    totalTime+=ticks;
    if(!stop)
    {
        std::stringstream strm;
        if(countDown) 
        {
            if((startTime-totalTime/1000)==0)
            {
                pause();
            }
            strm << std::setfill('0') << std::setw(2) << (startTime - (totalTime/1000))/60 << ":"
                 << std::setfill('0') << std::setw(2) << (startTime - (totalTime/1000))%60 
                 << std::endl;
        }
        else if(!countDown)
        {
            strm << std::setfill('0') << std::setw(2) << totalTime/1000/60 << ":" 
                << std::setfill('0') << std::setw(2) << totalTime/1000%60 
                << std::endl;
        }
        strm >> time;
    }
}
