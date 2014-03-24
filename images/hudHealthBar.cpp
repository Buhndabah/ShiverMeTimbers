#include "hudHealthBar.h"

HUDHealthBar::HUDHealthBar(const string& name, const Vector2f& pos, bool vis, const GridElement& pl) :
    HUDComponent(name, pos, vis),
    player(pl)
{ }

HUDHealthBar::HUDHealthBar(const HUDHealthBar& rhs) :
    HUDComponent(rhs),
    player(rhs.player)
{ }

HUDHealthBar& operator=(const HUDHealthBar& rhs) :
    HUDComponent::operator=(rhs);
    player = rhs.player;
}

void draw() const {
    bar->draw();
}

void update(Uint32 ticks) {
}
