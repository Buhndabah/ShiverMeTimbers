#ifndef HUDBUTTON_H
#define HUDBUTTON_H
#include "hudComponent.h"

#include "hudImage.h"
#include "hudText.h"

class HUDButton: public HUDComponent {
public:
    HUDButton(const std::string& n, const Vector2f& p, bool v, const std::string& sp, const std::string& text, const std::string& t);
    HUDButton(const HUDButton& rhs);
    HUDButton& operator=(const HUDButton& rhs);
    virtual ~HUDButton();

    virtual void draw() const;
    virtual void update(Uint32);

    virtual void setPosition(const Vector2f& newPos);

    Vector2f getSize() const;

    virtual void onClick(const GameEvents::Event* e);
    virtual void onWin(const GameEvents::Event* e);
    virtual void onLose(const GameEvents::Event* e);

private:
    HUDImage* image;
    HUDText* text;

    std::string type;
    bool active;

    virtual void registerListeners();
};
void HUDButtonClickForwarder(Listener*, const GameEvents::Event*);
void HUDButtonWinForwarder(Listener*, const GameEvents::Event*);
void HUDButtonLoseForwarder(Listener*, const GameEvents::Event*);
#endif
