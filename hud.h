#ifndef HUD_H
#define HUD_H
#include <list>
#include <map>
#include "hudComponent.h"
#include "hudContainer.h"
#include "gridElement.h"
#include "xmlparser.h"

class HUD {
public:
    static HUD& getInstance();

    ~HUD(); 

    void setPlayer(GridElement* pl);
    void toggleFade() { fade = !fade; }
    void toggle() {visible=!visible;}
    void toggleHelp() const;

    void draw() const;
    void update(Uint32) const;
    void onPause(unsigned int) const;

    void addComponent(HUDComponent*);
    void addTextComponent(const std::string&,const Vector2f&,const std::string&, bool);
    void addFPS(const Vector2f&);
    void setComponentText(const std::string&, const std::string&) const;


private:
    HUD(const std::string& fn = "xmlSpec/hud.xml"); 
    HUD(const HUD&);
    HUD& operator=(const HUD&);

    void parseComponents();

    XMLParser& parser;
    std::list<HUDComponent*> components;
    HUDComponent* createComponent(std::map<std::string, std::string> );
    HUDComponent* createComponent(std::map<std::string, std::string>, HUDContainer*);
    bool visible;
    bool fade;
    GridElement *player;

    enum {
        UNPAUSE,
        PAUSE
    };
};
#endif
