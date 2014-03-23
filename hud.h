#ifndef HUD_H
#define HUD_H
#include <list>
#include "hudComponent.h"

class HUD {
public:
    static HUD& getInstance();

    void toggleFade() { fade = !fade; }
    void toggle() {visible=!visible;};
    void draw() const;
    void update(Uint32) const;
    void addComponent(HUDComponent*);
    void addTextComponent(const Vector2f&,const std::string&, bool);
    void addFPS(const Vector2f&);
    void onPause() const;
private:
    HUD(); 
    HUD(const HUD&);
    HUD& operator=(const HUD&);

    std::list<HUDComponent*> components;
    bool visible;
    bool fade;
};
#endif
