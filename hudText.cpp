#include "hudText.h"

HUDText::HUDText(const Vector2f& p, bool v) :
    HUDComponent(p,v),
    io(IOManager::getInstance()),
    text("")
{ }

HUDText::HUDText(const Vector2f& p, bool v, const std::string s) :
    HUDComponent(p,v),
    io(IOManager::getInstance()),
    text(s)
{ }

HUDText& HUDText::operator=(const HUDText& rhs)
{
    HUDComponent::operator=(rhs);
    text = rhs.text;
    return *this;
}

void HUDText::draw() const 
{
    io.printMessageAt(text, getPosition()[0], getPosition()[1]);
}

void HUDText::update(Uint32 ticks) {
    // nothing
}

void HUDText::setText(const std::string &t)
{
    text = t;
}
