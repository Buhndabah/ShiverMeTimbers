#ifndef HUDCOMPONENT_H
#define HUDCOMPONENT_H
#include <string>
#include "listener.h"
#include "sprite.h"
#include "vector2f.h"

class HUDComponent : public Listener{
public:
    HUDComponent(const std::string& n, const Vector2f& p,bool v) : 
        Listener(),
        name(n),
        position(p), 
        visible(v),
        visibleWhenPaused(false),
        visibleNotPause(true),
        visibleWin(false),
        visibleLose(false),
        flicker(false),
        flickerTime(0.0),
        elapsedTicks(0)
    {}
    HUDComponent(const HUDComponent& rhs) :
        Listener(rhs),
        name(rhs.name),
        position(rhs.position),
        visible(rhs.visible),
        visibleWhenPaused(rhs.visibleWhenPaused),
        visibleNotPause(rhs.visibleNotPause),
        visibleWin(rhs.visibleWin),
        visibleLose(rhs.visibleLose),
        flicker(rhs.flicker),
        flickerTime(rhs.flickerTime),
        elapsedTicks(rhs.elapsedTicks)
    {} 

    virtual ~HUDComponent() {}

    virtual void draw() const=0;
    virtual void update(Uint32 ticks) { elapsedTicks+=ticks; }

    bool isVisible() const { return visible; }
    virtual void setVisible(bool v) { visible =v; }

    bool isVisibleWhenPaused() const { return visibleWhenPaused; }
    void setVisibleWhenPaused(bool v) { visibleWhenPaused = v; }

    bool isVisibleNotPaused() const { return visibleNotPause; }
    void setVisibleNotPaused(bool v) { visibleNotPause = v; }

    bool isVisibleWin() const { return visibleWin; }
    void setVisibleWin(bool v) { visibleWin = v; }

    bool isVisibleLose() const { return visibleLose; }
    void setVisibleLose(bool v) { visibleLose = v; }

    const Vector2f& getPosition() const { return position; }
    virtual void setPosition(const Vector2f& newPos) { position = newPos; }
    
    bool hasFlicker() const { return flicker; }
    void setFlicker(bool v) { flicker =v; }

    int timeToFlicker() const { return flickerTime; }
    void setTimeToFlicker(int time) { flickerTime = time; }

    const std::string& getName() const { return name; }
    Uint32 getElapsedTicks() const { return elapsedTicks; }
    void resetTicks() { elapsedTicks =0; }
private:
    virtual void registerListeners() {}

    std::string name;
    Vector2f position;
    bool visible;
    bool visibleWhenPaused;
    bool visibleNotPause;
    bool visibleWin;
    bool visibleLose;
    bool flicker;
    double flickerTime;
    Uint32 elapsedTicks;
};
#endif
