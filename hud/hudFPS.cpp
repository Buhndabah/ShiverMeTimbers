#include "hudFPS.h"

HUDFPS::HUDFPS(const std::string&n, const Vector2f& p, bool v) :
    HUDComponent(n,p,v),
    io(IOManager::getInstance()),
    clock(Clock::getInstance())
{ }

HUDFPS::HUDFPS(const HUDFPS& rhs) :
    HUDComponent(rhs),
    io(IOManager::getInstance()),
    clock(Clock::getInstance())
{ }

HUDFPS& HUDFPS::operator=(const HUDFPS& rhs)
{
    HUDComponent::operator=(rhs);
    return *this;
}

void HUDFPS::draw() const
{   
    if(isVisible())
    {
        io.printMessageValueAt("fps: ", clock.getFps(), getPosition()[0],getPosition()[1]);
    }
}

void HUDFPS::update(Uint32 ticks) 
{
    (void)ticks;
}
