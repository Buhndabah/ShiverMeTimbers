#ifndef HUDCLOCK_H
#define HUDCLOCK_H
#include "hudComponent.h"
#include "clock.h"
#include "ioManager.h"
#include "vector2f.h"

class HUDClock : public HUDComponent {
public:
    HUDClock(const Vector2f& p,bool v);
    HUDClock(const HUDClock& rhs);
    virtual ~HUDClock() {}
    virtual void draw() const;
    virtual void update(Uint32);
private:
    const IOManager& io;
    const Clock& clock;
};
#endif
