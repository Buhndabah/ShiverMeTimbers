#ifndef DUMMYCOMPONENT_H
#define DUMMYCOMPONENT_H
#include "hudComponent.h"
#include "vector2f.h"

class DummyComponent : public HUDComponent {
public:
    DummyComponent();
    DummyComponent(Vector2f& pos, bool v);

    virtual ~DummyComponent() {}

    virtual void draw() const {}
    virtual void update(Uint32 ticks) {}
};
#endif
