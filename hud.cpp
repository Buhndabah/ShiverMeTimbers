#include "hud.h"
#include "dummyComponent.h"
#include "hudClock.h"
HUD& HUD::getInstance() {
    static HUD instance;
    return instance;
}

HUD::HUD() :
    components(),
    visible(true)
{ components.push_back(new HUDClock(Vector2f(100,50),true, 600));}

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

void HUD::addComponent(HUDComponent *c) {
    components.push_back(c);
}
