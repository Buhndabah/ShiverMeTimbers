#include "hudHealthBar.h"


HUDHealthBar::HUDHealthBar(const std::string& name, const Vector2f& pos, bool vis,const std::string& sp) :
    HUDComponent(name, pos, vis),
    bar(new HUDImage(name,pos,vis,sp)),
    healthRatio(1),
    offset(pos)
{
    GameEvents::EventQueue::getInstance().addListener(GameEvents::DAMAGE_EVENT, static_cast<Listener*>(this), &HUDHPDamageForwarder); 
    GameEvents::EventQueue::getInstance().addListener(GameEvents::MOVE_EVENT, static_cast<Listener*>(this), &HUDHPMoveForwarder); 
}

HUDHealthBar::HUDHealthBar(const HUDHealthBar& rhs) :
    HUDComponent(rhs),
    bar(rhs.bar),
    healthRatio(rhs.healthRatio),
    offset(rhs.offset)
{ }

// forwarding function for damage events
void HUDHPDamageForwarder(Listener* context, const GameEvents::Event e) {
    static_cast<HUDHealthBar*>(context)->onDamage(e);
}

// forwarding function for move events
void HUDHPMoveForwarder(Listener* context, const GameEvents::Event e) {
    static_cast<HUDHealthBar*>(context)->onMove(e);
}

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

void HUDHealthBar::onDamage(const GameEvents::Event e) {
    if(e.actor.compare(getName())==0)
    {
        healthRatio = e.data.back();
    }
}

void HUDHealthBar::onMove(const GameEvents::Event e) {
    if(e.actor.compare(getName())==0)
    {
        bar->setPosition(e.location+offset);
    }
}

void HUDHealthBar::update(Uint32 ticks) {
    (void)ticks;
}
