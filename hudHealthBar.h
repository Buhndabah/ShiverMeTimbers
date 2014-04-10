#ifndef HUDHEALTH_H
#define HUD_HEALTH
#include "gridElement.h"
#include "hudComponent.h"
#include "hudImage.h"

class HUDHealthBar : public HUDComponent {
public:
    HUDHealthBar(const std::string& name, const Vector2f& pos, bool vis, const std::string& sp);
    HUDHealthBar(const HUDHealthBar& rhs);
    HUDHealthBar& operator=(const HUDHealthBar& rhs);
    virtual ~HUDHealthBar() {delete bar;}

    virtual void draw() const;
    virtual void update(Uint32);


private:
    HUDImage* bar;
    float healthRatio;
    Vector2f offset;
};
#endif
