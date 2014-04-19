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

    virtual void onDamageReceived(const GameEvents::DamageReceivedEvent *);
    virtual void onMove(const GameEvents::Event *);

private:
    virtual void registerListeners();

    HUDImage* bar;
    float healthRatio;
    Vector2f offset;
};

void HUDHPDamageForwarder(Listener*, const GameEvents::Event*);
void HUDHPMoveForwarder(Listener*, const GameEvents::Event*);
#endif
