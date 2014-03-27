#ifndef HUDCOMPONENT_H
#define HUDCOMPONENT_H
#include <string>
#include "sprite.h"
#include "vector2f.h"

class HUDComponent {
public:
    HUDComponent(const std::string& n, const Vector2f& p,bool v) : 
        name(n),
        position(p), 
        visible(v),
        visibleWhenPaused(false),
        visibleNotPause(true)
    {}
    HUDComponent(const HUDComponent& rhs) :
        name(rhs.name),
        position(rhs.position),
        visible(rhs.visible),
        visibleWhenPaused(rhs.visibleWhenPaused),
        visibleNotPause(rhs.visibleNotPause)
    {} 

    virtual ~HUDComponent() {}

    virtual void draw() const=0;
    virtual void update(Uint32 ticks) =0;

    bool isVisible() const { return visible; }
    virtual void setVisible(bool v) { visible =v; }
    bool isVisibleWhenPaused() const { return visibleWhenPaused; }
    void setVisibleWhenPaused(bool v) { visibleWhenPaused = v; }
    bool isVisibleNotPaused() const { return visibleNotPause; }
    void setVisibleNotPaused(bool v) { visibleNotPause = v; }

    const Vector2f& getPosition() const { return position; }
    const std::string& getName() const { return name; }
private:
    std::string name;
    Vector2f position;
    bool visible;
    bool visibleWhenPaused;
    bool visibleNotPause;
};
#endif
