#include "hud.h"
#include "gamedata.h"
#include "dummyComponent.h"
#include "hudClock.h"
#include "hudText.h"
#include "hudFPS.h"
#include "hudImage.h"

HUD& HUD::getInstance() {
    static HUD instance;
    return instance;
}

HUD::HUD() :
    components(),
    visible(true),
    fade(false)
{ 
    components.push_back(new HUDClock("clock", Vector2f(10,30),true, 600));
    components.push_back(new HUDText("testText", Vector2f(375,600),true,"TEST TEXT CYKA CYKA BLYAT",false));
    addComponent(new HUDImage("pause screen", Vector2f(0,0), false, "pauseScreen"));
}

void HUD::draw() const {
    if(visible) {
        for(std::list<HUDComponent*>::const_iterator it = components.begin();it!=components.end();++it)
        {
            (*it)->draw();
        }
    }
}

void HUD::onPause() const {
    for(std::list<HUDComponent*>::const_iterator it=components.begin();it!=components.end();++it)
    {
        (*it)->setVisible();
    }
}

void HUD::update(Uint32 ticks) const {
    for(std::list<HUDComponent*>::const_iterator it=components.begin();it!=components.end();++it)
    {
        (*it)->update(ticks);
    }
}

void HUD::addTextComponent(const std::string& name, const Vector2f& pos, const std::string& text, bool c) 
{
    addComponent(new HUDText(name, pos,true, text, c));
}

void HUD::addFPS(const Vector2f& pos)
{
    addComponent(new HUDFPS("fps", pos, true));
}

void HUD::setComponentText(const std::string& name, const std::string& text) const {
    for(std::list<HUDComponent*>::const_iterator it=components.begin();it!=components.end();++it)
    {
        if(strcmp((*it)->getName().c_str(),name.c_str())==0)
        {
            ((HUDText*)(*it))->setText(text);
        }
    }
}

void HUD::addComponent(HUDComponent *c) {
    components.push_back(c);
}
