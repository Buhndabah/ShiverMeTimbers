#include "hud.h"
#include "gamedata.h"
#include "dummyComponent.h"
#include "hudClock.h"
#include "hudText.h"
#include "hudFPS.h"
#include "hudImage.h"
#include "hudHealthBar.h"
#include "hudScore.h"

HUD& HUD::getInstance() {
    static HUD instance;
    return instance;
}

HUD::HUD(const std::string& fn) :
    parser(XMLParser::getInstance()),
    components(),
    visible(true),
    fade(false)
{ 
    registerListeners();
    parser.parse(fn);
    parseComponents();
}

HUD::~HUD() 
{
    while(!components.empty())
    {
        delete components.front();
        components.pop_front();
    }
}

// add a health bar to entity with name n
void HUD::addHealthBar(const std::string& n, const Vector2f& offset) {
    addComponent(new HUDHealthBar(n, offset, true, "healthBar"));
}

// toggle each component's visibility based on game state
void HUD::onPause(unsigned int state) const {
    for(std::list<HUDComponent*>::const_iterator it=components.begin();it!=components.end();++it)
    {
        if(state == PAUSE) 
        {
            if((*it)->isVisibleWhenPaused())
            {
                (*it)->setVisible(true);
            }
            else
            {
                (*it)->setVisible(false);
            }
        }
        else if(state == UNPAUSE)
        {
            if((*it)->isVisibleWhenPaused())
            {
                (*it)->setVisible(false);
            }
            else if((*it)->isVisibleNotPaused())
            {
                (*it)->setVisible(true);
            }
        }
    }
}

// draw each component
void HUD::draw() const {
    if(visible) {
        for(std::list<HUDComponent*>::const_iterator it = components.begin();it!=components.end();++it)
        {
            (*it)->draw();
        }
    }
}

// Update each component
void HUD::update(Uint32 ticks) const {
    for(std::list<HUDComponent*>::const_iterator it=components.begin();it!=components.end();++it)
    {
        (*it)->update(ticks);
    }
}

// Sets text on components (only supports HUDText right now)
void HUD::setComponentText(const std::string& name, const std::string& text) const {
    for(std::list<HUDComponent*>::const_iterator it=components.begin();it!=components.end();++it)
    {
        if(strcmp((*it)->getName().c_str(),name.c_str())==0)
        {
            static_cast<HUDText*>(*it)->setText(text);
        }
    }
}

// Turns the help dialogue on/off
void HUD::toggleHelp() const {
    for(std::list<HUDComponent*>::const_iterator it = components.begin(); it!= components.end(); ++it)
    {
        if(strcmp((*it)->getName().c_str(),"help")==0)
        {
            (*it)->setVisible(!(*it)->isVisible());
        }
    }
}

// Push new component onto list
void HUD::addComponent(HUDComponent *c) {
    components.push_back(c);
}

// Publically visible constructor for a text box
void HUD::addTextComponent(const std::string& name, const Vector2f& pos, const std::string& text, bool visible) {
    addComponent(new HUDText(name, pos, true, text, visible));
} 


/* Create a component and add it to HUD's component list. Returns a pointer to that component. */
HUDComponent* HUD::createComponent(std::map<std::string, std::string> componentParams)
{

        // Check type
        if(componentParams["type"].compare("clock")==0)
        {
            addComponent(new HUDClock(componentParams["name"],
                                      Vector2f(atoi(componentParams["x"].c_str()),atoi(componentParams["y"].c_str())),
                                      componentParams["visible"].compare("true") ? 0 : 1,
                                      atoi(componentParams["start"].c_str())));
        }
        else if(componentParams["type"].compare("text")==0)
        {
            if(componentParams.find("font")!=componentParams.end() && componentParams.find("color")!=componentParams.end())
            {
                //std::cerr<< componentParams["font"] << " " << componentParams["color"] << std::endl;
                addComponent(new HUDText(componentParams["name"],
                                         Vector2f(atoi(componentParams["x"].c_str()),atoi(componentParams["y"].c_str())),
                                         componentParams["visible"].compare("true") ? 0 : 1, 
                                         componentParams["text"],
                                         componentParams["centered"].compare("true") ? 0 : 1, componentParams["font"],componentParams["color"]));
            }
            else
            {
                addComponent(new HUDText(componentParams["name"],
                                         Vector2f(atoi(componentParams["x"].c_str()),atoi(componentParams["y"].c_str())),
                                         componentParams["visible"].compare("true") ? 0 : 1, 
                                         componentParams["text"],
                                         componentParams["centered"].compare("true") ? 0 : 1));
            }
        }
        else if(componentParams["type"].compare("fps")==0)
        {
            addComponent(new HUDFPS(componentParams["name"],
                                    Vector2f(atoi(componentParams["x"].c_str()),atoi(componentParams["y"].c_str())),
                                    componentParams["visible"].compare("true") ? 0 : 1));
        }
        else if(componentParams["type"].compare("image")==0)
        {
            addComponent(new HUDImage(componentParams["name"],
                                      Vector2f(atoi(componentParams["x"].c_str()),atoi(componentParams["y"].c_str())),
                                      componentParams["visible"].compare("true") ? 0 : 1,
                                      componentParams["file"]));
        }
        else if(componentParams["type"].compare("container")==0)
        {
            addComponent(new HUDContainer(componentParams["name"], 
                                          Vector2f(atoi(componentParams["x"].c_str()),atoi(componentParams["y"].c_str())), 
                                          componentParams["visible"].compare("true") ? 0 : 1));
        }
        else if(componentParams["type"].compare("score")==0)
        {
            std::cerr<< "added score" << std::endl;
            addComponent(new HUDScore(componentParams["name"],
                                      Vector2f(atoi(componentParams["x"].c_str()),atoi(componentParams["y"].c_str())),
                                      componentParams["visible"].compare("true") ? 0 : 1));
        }

        // Check optional parameters
        if(componentParams.find("visibleNotPaused") != componentParams.end())
        {
            components.back()->setVisibleNotPaused(componentParams["visibleNotPaused"].compare("true") ? 0 : 1);
        }
        if(componentParams.find("visibleWhenPaused") != componentParams.end())
        {
            components.back()->setVisibleWhenPaused(componentParams["visibleWhenPaused"].compare("true") ? 0 : 1);
        }
        if(componentParams.find("flicker") != componentParams.end())
            {
                components.back()->setFlicker(componentParams["flicker"].compare("true") ? 0 : 1);
            }
        if(componentParams.find("flickerTime") != componentParams.end())
        {
            std::cerr << " loaded flicker time " << atoi(componentParams["flickerTime"].c_str()) << std::endl;
            components.back()->setFlicker(atoi(componentParams["flickerTime"].c_str()));
        }


        return components.back();
}

/* Create a component, and add it to HUDContainer's component list. Returns a pointer to that component. */
HUDComponent* HUD::createComponent(std::map<std::string, std::string> componentParams, HUDContainer* cont)
{

        // Check type
        if(componentParams["type"].compare("clock")==0)
        {
            cont->addComponent(new HUDClock(componentParams["name"],
                                            Vector2f(atoi(componentParams["x"].c_str()),atoi(componentParams["y"].c_str())),
                                            componentParams["visible"].compare("true") ? 0 : 1,
                                            atoi(componentParams["start"].c_str())));
        }
        else if(componentParams["type"].compare("text")==0)
        {
            if(componentParams.find("font")!=componentParams.end() && componentParams.find("color")!=componentParams.end())
            {
                //std::cerr<< componentParams["font"] << " " << componentParams["color"] << std::endl;
                cont->addComponent(new HUDText(componentParams["name"],
                                               Vector2f(atoi(componentParams["x"].c_str()),atoi(componentParams["y"].c_str())),
                                               componentParams["visible"].compare("true") ? 0 : 1, 
                                               componentParams["text"],
                                               componentParams["centered"].compare("true") ? 0 : 1, componentParams["font"],componentParams["color"]));
            }
            else
            {
                cont->addComponent(new HUDText(componentParams["name"],
                                               Vector2f(atoi(componentParams["x"].c_str()),atoi(componentParams["y"].c_str())),
                                               componentParams["visible"].compare("true") ? 0 : 1, 
                                               componentParams["text"],
                                               componentParams["centered"].compare("true") ? 0 : 1));
            }
        }
        else if(componentParams["type"].compare("fps")==0)
        {
            cont->addComponent(new HUDFPS(componentParams["name"],
                                          Vector2f(atoi(componentParams["x"].c_str()),atoi(componentParams["y"].c_str())),
                                          componentParams["visible"].compare("true") ? 0 : 1));
        }
        else if(componentParams["type"].compare("image")==0)
        {
            cont->addComponent(new HUDImage(componentParams["name"],
                                      Vector2f(atoi(componentParams["x"].c_str()),atoi(componentParams["y"].c_str())),
                                      componentParams["visible"].compare("true") ? 0 : 1,
                                      componentParams["file"]));
        }
        else if(componentParams["type"].compare("container")==0)
        {
            cont-> addComponent(new HUDContainer(componentParams["name"], 
                                                 Vector2f(atoi(componentParams["x"].c_str()),atoi(componentParams["y"].c_str())), 
                                                 componentParams["visible"].compare("true") ? 0 : 1));
        }
        else if(componentParams["type"].compare("score")==0)
        {
            cont->addComponent(new HUDScore(componentParams["name"],
                                            Vector2f(atoi(componentParams["x"].c_str()),atoi(componentParams["y"].c_str())),
                                            componentParams["visible"].compare("true") ? 0 : 1));
        }

        // Check optional parameters
        if(componentParams.find("visibleNotPaused") != componentParams.end())
        {
            cont->back()->setVisibleNotPaused(componentParams["visibleNotPaused"].compare("true") ? 0 : 1);
        }
        if(componentParams.find("visibleWhenPaused") != componentParams.end())
        {
            cont->back()->setVisibleWhenPaused(componentParams["visibleWhenPaused"].compare("true") ? 0 : 1);
        }
        if(componentParams.find("flicker") != componentParams.end())
            {
                cont->back()->setFlicker(componentParams["flicker"].compare("true") ? 0 : 1);
            }
        if(componentParams.find("flickerTime") != componentParams.end())
        {
            cont->back()->setTimeToFlicker(atoi(componentParams["flickerTime"].c_str()));
        }
        return cont->back();
}

/* Read in components from xml file */
void HUD::parseComponents() {
    parser.setCurDocument("xmlSpec/hud.xml");

    std::list<std::map<std::string, std::string> > componentList = parser.parseNodesWithTag("component");   // components are immediately ready to parse in...

    std::list<const rapidxml::xml_node<>* > containerList=parser.findNodes("container");    // ...containers take a bit more work though
    std::map<std::string,std::string> containerParams;
    HUDContainer* newContainer = NULL;

    for(std::list<std::map<std::string, std::string> >::const_iterator it=componentList.begin(); it!=componentList.end(); ++it)
    {
        createComponent(*it);
    }

    // walk through container entries
    for(std::list<const rapidxml::xml_node<>*>::const_iterator contIt = containerList.begin(); contIt!= containerList.end(); ++contIt)
    {
        containerParams=parser.parseNode(*contIt);
        newContainer = static_cast<HUDContainer*>(createComponent(containerParams));
        componentList = parser.parseNodesWithTag("item", *contIt);

        // Walk through components listed for each container
        for(std::list<std::map<std::string, std::string> >::const_iterator contComp = componentList.begin(); contComp != componentList.end(); ++contComp)
         {
             createComponent(*contComp, newContainer);
         }
    }

    parser.removeDoc("xmlSpec/hud.xml");
} 

/********** Event Handling Methods ************/

void HUD::onHUDRemove(const GameEvents::Event *e) {
    for(std::list<HUDComponent*>::iterator it = components.begin(); it!=components.end(); ++it)
    {
        if( (*it)->getName().compare(e->getSource())==0)
        {
            delete (*it);
            components.erase(it);
            return;
        }
    }
}

void HUD::registerListeners() {
    GameEvents::EventQueue::getInstance().addListener(GameEvents::HUDREMOVE_EVENT, static_cast<Listener*>(this), &HUDRemoveForwarder);
}

void HUDRemoveForwarder(Listener* context, const GameEvents::Event *e) {
    dynamic_cast<HUD*>(context)->onHUDRemove(e);
}
