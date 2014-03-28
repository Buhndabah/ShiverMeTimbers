#include "hudText.h"

HUDText::HUDText(const std::string& n, const Vector2f& p, bool v) :
    HUDComponent(n,p,v),
    io(IOManager::getInstance()),
    text(""),
    centered(false),
    fontName(""),
    fontColor("")
{ }

HUDText::HUDText(const std::string& n, const Vector2f& p, bool v, const std::string s, bool c) :
    HUDComponent(n, p,v),
    io(IOManager::getInstance()),
    text(s),
    centered(c),
    fontName(""),
    fontColor("")
{ }

HUDText::HUDText(const std::string& n, const Vector2f& p, bool v, const std::string s, bool c, const std::string& font, const std::string& color) :
    HUDComponent(n,p,v),
    io(IOManager::getInstance()),
    text(s),
    centered(c),
    fontName(font),
    fontColor(color)
{ }

HUDText& HUDText::operator=(const HUDText& rhs)
{
    HUDComponent::operator=(rhs);
    text = rhs.text;
    centered=rhs.centered;
    fontName = rhs.fontName;
    fontColor = rhs.fontColor;
    return *this;
}

void HUDText::draw() const 
{
    if(isVisible())
    {
        if(centered)
        {
            if(fontName.compare("") == 0 && fontColor.compare("")==0)
            {
                io.printMessageCenteredAt(text,getPosition()[1]);
            }
            else
            {
                io.printMessageCenteredAt(text,getPosition()[1],fontName,fontColor);
            }
        }
        else
        {
            if(fontName.compare("") == 0 && fontColor.compare("")==0)
            {
                io.printMessageAt(text, getPosition()[0], getPosition()[1]);
            }
            else
            {
                io.printMessageAt(text,getPosition()[0], getPosition()[1],fontName,fontColor);
            }
        }
    }
}

void HUDText::update(Uint32 ticks) {
    (void)ticks;
}

void HUDText::setText(const std::string &t)
{
    text = t;
}
