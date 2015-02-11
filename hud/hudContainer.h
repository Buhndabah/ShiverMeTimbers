#ifndef HUDCONTAINER_H
#define HUDCONTAINER_H
#include <list>
#include "hudComponent.h"

class HUDContainer : public HUDComponent {
public:
    HUDContainer(const std::string& n, const Vector2f& pos, bool vis);
    HUDContainer(const std::string& n, const Vector2f& pos, bool vis, const std::list<HUDComponent*> comps);
    HUDContainer(const HUDContainer& rhs);
    HUDContainer& operator=(const HUDContainer& rhs);
    virtual ~HUDContainer();

    virtual void draw() const;
    virtual void update(Uint32);

    HUDComponent* back() const { return components.back(); }
    void addComponent(HUDComponent*);
    void removeComponent(const std::string&);
private:
    std::list<HUDComponent *> components;
};
#endif
