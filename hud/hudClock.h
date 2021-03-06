#ifndef HUDCLOCK_H
#define HUDCLOCK_H
#include "hudComponent.h"
#include "clock.h"
#include "ioManager.h"
#include "vector2f.h"

class HUDClock : public HUDComponent {
public:
    HUDClock(const std::string& n, const Vector2f& p, bool v, int i);
    HUDClock(const std::string& n, const Vector2f& p,bool v);
    HUDClock(const HUDClock& rhs);
    HUDClock& operator=(const HUDClock&);
    virtual ~HUDClock() {}
    virtual void draw() const;
    virtual void update(Uint32);
    void pause() { stop = true; }
    void play() { stop = false; }
private:
    const IOManager& io;
    const Clock& clock;
    Uint32 totalTime;
    std::string time;
    int startTime;  // in seconds
    bool countDown; // true if clock counts down
    bool stop;
};
#endif
