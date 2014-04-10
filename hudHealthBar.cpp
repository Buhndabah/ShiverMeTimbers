#include "gameEvents.h"
#include "hudHealthBar.h"

HUDHealthBar::HUDHealthBar(const std::string& name, const Vector2f& pos, bool vis,const std::string& sp) :
    HUDComponent(name, pos, vis),
    bar(new HUDImage(name,pos,vis,sp)),
    healthRatio(1),
    offset(pos)
{ 
}


HUDHealthBar::HUDHealthBar(const HUDHealthBar& rhs) :
    HUDComponent(rhs),
    bar(rhs.bar),
    healthRatio(rhs.healthRatio),
    offset(rhs.offset)
{ }

// I think this should be ok, why would we ever want to make multiple health bars anyway?
// might want to make into a singleton
HUDHealthBar& HUDHealthBar::operator=(const HUDHealthBar& rhs) 
{
    HUDComponent::operator=(rhs);
    bar = rhs.bar;
    healthRatio=rhs.healthRatio;
    offset = rhs.offset;
    return *this;
}

void HUDHealthBar::draw() const {
    if(healthRatio != 1) {
        bar->scaledDraw(healthRatio,1.0);
    }
}

void HUDHealthBar::update(Uint32 ticks) {
    (void)ticks;

    // Check to see if there are any events from our gridElement waiting
    std::vector<GameEvents::Event> events = GameEvents::EventQueue::getInstance().findEventsByActor(getName());
    if(events.size() !=0)
    {
        for(std::vector<GameEvents::Event>::const_iterator it = events.begin(); it!=events.end(); ++it)
        {
            if((*it).type == GameEvents::DAMAGE_EVENT)
            {
                healthRatio = (*it).data.back();
            }
            else if((*it).type == GameEvents::MOVE_EVENT)
            {
                bar->setPosition((*it).location + offset);
            }
        }
    }
}
