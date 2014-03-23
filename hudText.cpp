#include "hudText.h"

HUDText::HUDText(const std::string& n, const Vector2f& p, bool v) :
    HUDComponent(n,p,v),
    io(IOManager::getInstance()),
    text("")
{ }

HUDText::HUDText(const std::string& n, const Vector2f& p, bool v, const std::string s, bool c) :
    HUDComponent(n, p,v),
    io(IOManager::getInstance()),
    text(s),
    centered(c)
{ }

HUDText& HUDText::operator=(const HUDText& rhs)
{
    HUDComponent::operator=(rhs);
    text = rhs.text;
    centered=rhs.centered;
    return *this;
}

void HUDText::draw() const 
{
    if(centered)
    {
        io.printMessageCenteredAt(text,getPosition()[0]);
    }
    else
    {
        io.printMessageAt(text, getPosition()[0], getPosition()[1]);
    }
}

void HUDText::update(Uint32 ticks) {
    // nothing
}

void HUDText::setText(const std::string &t)
{
    text = t;
}
