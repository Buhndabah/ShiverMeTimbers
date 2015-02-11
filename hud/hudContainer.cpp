#include "hudContainer.h"

HUDContainer::HUDContainer(const std::string& n, const Vector2f& pos, bool vis) :
    HUDComponent(n,pos,vis),
    components()
{ }

HUDContainer::HUDContainer(const std::string& n, const Vector2f& pos, bool vis, const std::list<HUDComponent*> comps) :
    HUDComponent(n,pos,vis),
    components(comps)
{ }

HUDContainer::HUDContainer(const HUDContainer& rhs) :
    HUDComponent(rhs),
    components(rhs.components)
{ }

HUDContainer& HUDContainer::operator=(const HUDContainer& rhs)
{
    HUDComponent::operator=(rhs);
    for(std::list<HUDComponent*>::const_iterator it = rhs.components.begin(); it != rhs.components.end(); ++it)
    {
        components.push_back((*it));
    }
    return *this;
}

HUDContainer::~HUDContainer() {
    while(!components.empty())
    {
        delete components.front();
        components.pop_front();
    }
}

void HUDContainer::draw() const {
    if(isVisible())
    {
        for(std::list<HUDComponent*>::const_iterator it = components.begin(); it!= components.end(); ++it)
        {
            (*it)->draw();
        }
    }
}

void HUDContainer::update(Uint32 ticks) {
    for(std::list<HUDComponent*>::const_iterator it = components.begin(); it!= components.end(); ++it)
    {
        (*it)->update(ticks);
    }
}

void HUDContainer::addComponent(HUDComponent* rhs)
{
    components.push_back(rhs);
}

void HUDContainer::removeComponent(const std::string& name)
{
    for(std::list<HUDComponent*>::iterator it = components.begin(); it!= components.end();
    ++it)
    {
        if(strcmp((*it)->getName().c_str(),name.c_str())==0)
        {
            delete (*it);
            components.erase(it);
        }
    }
}

