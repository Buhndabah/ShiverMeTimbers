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
        visible(v) 
    {}
    HUDComponent(const HUDComponent& rhs) :
        name(rhs.name),
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
    std::string name;
    Vector2f position;
    bool visible;
};
#endif
