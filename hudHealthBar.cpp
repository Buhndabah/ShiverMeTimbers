#include "hudHealthBar.h"

HUDHealthBar::HUDHealthBar(const std::string& name, const Vector2f& pos, bool vis, GridElement* pl, const std::string& sp) :
    HUDComponent(name, pos, vis),
    player(pl),
    bar(new HUDImage(name,pos,vis,sp)),
    healthRatio(1)
{ }

HUDHealthBar::HUDHealthBar(const HUDHealthBar& rhs) :
    HUDComponent(rhs),
    player(rhs.player),
    bar(rhs.bar),
    healthRatio(rhs.healthRatio)
{ }

// I think this should be ok, why would we ever want to make multiple health bars anyway?
// might want to make into a singleton
HUDHealthBar& HUDHealthBar::operator=(const HUDHealthBar& rhs) 
{
    HUDComponent::operator=(rhs);
    player = rhs.player;
    bar = rhs.bar;
    healthRatio=rhs.healthRatio;
    return *this;
}

void HUDHealthBar::draw() const {
    bar->partialDraw(healthRatio,1.0);
}

void HUDHealthBar::update(Uint32 ticks) {
    healthRatio = (static_cast<double>(player->getCurrentHP())/static_cast<double>(player->getMaxHP()));
    std::cerr << "healthRatio is " << player->getCurrentHP()/player->getMaxHP() << std::endl;
}
