#ifndef HUDCOMPONENT_H
#define HUDCOMPONENT_H
#include "sprite.h"
#include "vector2f.h"

class HUDComponent {
public:
    HUDComponent(const Vector2f& p,bool v) : position(p), visible(v) {}
    HUDComponent(const HUDComponent& rhs) :
        position(rhs.position),
        visible(rhs.visible)
    {} 

    virtual ~HUDComponent() {}

    virtual void draw() const=0;
    virtual void update(Uint32 ticks) =0;
    bool isVisible() const { return visible; }
    virtual void setVisible() { visible =!visible; }

    const Vector2f& getPosition() const { return position; }

private:
    Vector2f position;
    bool visible;
};
#endif
