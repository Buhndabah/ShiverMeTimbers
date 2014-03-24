#include "hud.h"
#include "gamedata.h"
#include "dummyComponent.h"
#include "hudClock.h"
#include "hudContainer.h"
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
    components.push_back(new HUDClock("clock", Vector2f(10,30),true, 60));
    components.push_back(new HUDText("testText", Vector2f(375,600),true,"TEST TEXT CYKA CYKA BLYAT",false));

    // pause stuff
    addComponent(new HUDContainer("pause", Vector2f(0,0), false));
    components.back()->setVisibleWhenPaused(true);
    components.back()->setVisibleNotPause(false);
    ((HUDContainer*)components.back())->addComponent(new HUDImage("pause screen",Vector2f(0,0), true, "pauseScreen"));
    ((HUDContainer*)components.back())->addComponent(new HUDText("pause text", Vector2f(0,10),true,"Paused", true));

    // help dialogue stuff
    addComponent(new HUDContainer("help", Vector2f(0,0), false));
    ((HUDContainer*)components.back())->addComponent(new HUDImage("background", Vector2f(0,0),true,"controlPopUp"));
    components.back()->setVisibleNotPause(false);
    ((HUDContainer*)components.back())->addComponent(new HUDText("help text",Vector2f(10,400),true, "WASD to move",false));
}

HUD::~HUD() 
{
    for(std::list<HUDComponent*>::iterator it=components.begin(); it != components.end(); ++it)
    {
        delete (*it);
    }
}

void HUD::draw() const {
    if(visible) {
        for(std::list<HUDComponent*>::const_iterator it = components.begin();it!=components.end();++it)
        {
            (*it)->draw();
        }
    }
}

void HUD::onPause(unsigned int state) const {
    for(std::list<HUDComponent*>::const_iterator it=components.begin();it!=components.end();++it)
    {
        if(state == PAUSE) 
        {
            if((*it)->isVisibleWhenPaused())
            {
                (*it)->setVisible(true);
            }
            else
            {
                (*it)->setVisible(false);
            }
        }
        else if(state == UNPAUSE)
        {
            if((*it)->isVisibleWhenPaused())
            {
                (*it)->setVisible(false);
            }
            else if((*it)->isVisibleNotPause())
            {
                (*it)->setVisible(true);
            }
        }
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

void HUD::toggleHelp() const {
    for(std::list<HUDComponent*>::const_iterator it = components.begin(); it!= components.end(); ++it)
    {
        if(strcmp((*it)->getName().c_str(),"help")==0)
        {
            (*it)->setVisible(!(*it)->isVisible());
        }
    }
}

void HUD::addComponent(HUDComponent *c) {
    components.push_back(c);
}
