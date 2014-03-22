#include "dummyComponent.h"

DummyComponent::DummyComponent() : HUDComponent(Vector2f(0,0),true) {}
DummyComponent::DummyComponent(Vector2f& pos, bool v) : HUDComponent(pos, v) {}
