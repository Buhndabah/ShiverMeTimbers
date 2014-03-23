#include "hud.h"
#include "dummyComponent.h"
#include "hudClock.h"
#include "hudText.h"
#include "hudFPS.h"

HUD& HUD::getInstance() {
    static HUD instance;
    return instance;
}

HUD::HUD() :
    components(),
    visible(true),
    fade(false)
{ 
    components.push_back(new HUDClock("clock", Vector2f(100,50),true, 600));
    components.push_back(new HUDText("testText", Vector2f(100,75),true,"TEST TEXT CYKA CYKA BLYAT",false));
}

void HUD::draw() const {
    if(visible) {
        for(std::list<HUDComponent*>::const_iterator it = components.begin();it!=components.end();++it)
        {
            (*it)->draw();
        }
    }
}

void HUD::update(Uint32 ticks) const {
    for(std::list<HUDComponent*>::const_iterator it=components.begin();it!=components.end();++it)
    {
        (*it)->update(ticks);
    }
}

void HUD::addTextComponent(const Vector2f& pos, const std::string& text, bool c) 
{
    addComponent(new HUDText("text", pos,true, text, c));
}

void HUD::addFPS(const Vector2f& pos)
{
    addComponent(new HUDFPS("fps", pos, true));
}

void HUD::addComponent(HUDComponent *c) {
    components.push_back(c);
}
