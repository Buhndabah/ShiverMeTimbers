#include "dummyComponent.h"

DummyComponent::DummyComponent() : HUDComponent("dummy",Vector2f(0,0),true) {}
DummyComponent::DummyComponent(const std::string& n, const Vector2f& pos, bool v) : HUDComponent(n, pos, v) {}
