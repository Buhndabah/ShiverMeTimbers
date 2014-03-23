#ifndef HUD_H
#define HUD_H
#include <list>
#include "hudComponent.h"

class HUD {
public:
    static HUD& getInstance();
    HUD(const std::list<HUDComponent*> &l, bool b) : components(l), visible(b) {};
    HUD(const std::list<HUDComponent*> &l) : components(l),visible(true) {};


    void toggleFade() { fade = !fade; }
    void toggle() {visible=!visible;};
    void draw() const;
    void update(Uint32) const;
    void addComponent(HUDComponent*);

private:
    HUD(); 
    HUD(const HUD&);
    HUD& operator=(const HUD&);

    std::list<HUDComponent*> components;
    bool visible;
    bool fade;
};
#endif
