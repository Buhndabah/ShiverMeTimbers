#ifndef HUDHEALTH_H
#define HUD_HEALTH
#include "hudComponent.h"
#include "hudImage.h"

class HUDHealthBar : public HUDComponent {
public:
    HUDHealthBar(const string& name, const Vector2f& pos, bool vis, const GridElement* pl);
    HUDHealthBar(const HUDHealthBar& rhs);
    HUDHealthBar& operator=(const HUDHealthBar& rhs);
    virtual ~HUDHealthBar() {delete bar;}

    virtual void draw() const;
    virtual void update(Uint32);

private:
    GridElement* player;
    HUDImage* bar;
};
#endif
