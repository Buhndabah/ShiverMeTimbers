#ifndef HUD_H
#define HUD_H
#include <list>
#include "hudComponent.h"
#include "gridElement.h"

class HUD {
public:
    static HUD& getInstance();

    ~HUD(); 

    void setPlayer(GridElement* pl);
    void toggleFade() { fade = !fade; }
    void toggle() {visible=!visible;};
    void draw() const;
    void update(Uint32) const;
    void addComponent(HUDComponent*);
    void addTextComponent(const std::string&,const Vector2f&,const std::string&, bool);
    void addFPS(const Vector2f&);
    void onPause(unsigned int) const;
    void setComponentText(const std::string&, const std::string&) const;
    void toggleHelp() const;
private:
    HUD(); 
    HUD(const HUD&);
    HUD& operator=(const HUD&);

    std::list<HUDComponent*> components;
    bool visible;
    bool fade;
    GridElement *player;

    enum {
        UNPAUSE,
        PAUSE
    };
};
#endif
