#include "hudButton.h"

HUDButton::HUDButton(const std::string& n, const Vector2f& p, bool v, const std::string& sp, const std::string& text, const std::string& t) :
    HUDComponent(n,p,v),
    image(new HUDImage(n,p,v,sp)),
    text(new HUDText(n,p+image->getSize()/4,v,text, false)),
    type(t),
    active(false)
{
    registerListeners();
}

HUDButton::HUDButton(const HUDButton& rhs) :
    HUDComponent(rhs),
    image(new HUDImage(*(rhs.image))),
    text(new HUDText(*(rhs.text))),
    type(rhs.type),
    active(false)
{
    registerListeners();    
}

HUDButton& HUDButton::operator=(const HUDButton& rhs) {
    if(this == &rhs) return *this;

    HUDImage* tmpImage = image;
    HUDText* tmpText = text;

    HUDComponent::operator=(rhs);

    image = new HUDImage(*(rhs.image));
    text = new HUDText(*(rhs.text));

    delete tmpImage;
    delete tmpText;

    type = rhs.type;
    active= rhs.active;
    return *this;
}

HUDButton::~HUDButton() {
    delete image;
    delete text;
}

void HUDButton::draw() const {
    image->draw();
    text->draw();
}

void HUDButton::update(Uint32 ticks) {
    image->update(ticks);
    text->update(ticks);
}

void HUDButton::setPosition(const Vector2f& newPos) {
    image->setPosition(newPos);
    text->setPosition(newPos);
}

Vector2f HUDButton::getSize() const {
    return image->getSize();
}

/********** Event Handling Functions  **********/

void HUDButton::onClick(const GameEvents::Event* e) {
    Vector2f pos = e->getPosition();
    
    bool inX = pos[0] > getPosition()[0] && pos[0] < getPosition()[0] + image->getPosition()[0];
    bool inY = pos[1] > getPosition()[1] && pos[1] < getPosition()[1] + image->getPosition()[1];
    if(active) {
        std::cerr<<type << std::endl;
        if(inY && inX){
            if(type.compare("restart")==0)
            {
                GameEvents::EventQueue::getInstance().push(new GameEvents::GameEndEvent(e->getSource(), e->getPosition(), true));
            }
            else if(type.compare("end")==0)
            {
                GameEvents::EventQueue::getInstance().push(new GameEvents::GameEndEvent(e->getSource(), e->getPosition(), false));
            }

        }
    }
}

void HUDButton::onWin(const GameEvents::Event* e) {
    active= true;
}
void HUDButton::onLose(const GameEvents::Event* e) {
    active=true;
}
void HUDButton::registerListeners() {
    GameEvents::EventQueue::getInstance().addListener(GameEvents::CLICK_EVENT, static_cast<Listener*>(this), &HUDButtonClickForwarder);
    GameEvents::EventQueue::getInstance().addListener(GameEvents::WIN_EVENT, static_cast<Listener*>(this), &HUDButtonWinForwarder);
    GameEvents::EventQueue::getInstance().addListener(GameEvents::LOSE_EVENT, static_cast<Listener*>(this), &HUDButtonLoseForwarder);
}

void HUDButtonClickForwarder(Listener* context, const GameEvents::Event* e)
{
    dynamic_cast<HUDButton*>(context)->onClick(e);
}
void HUDButtonWinForwarder(Listener* context, const GameEvents::Event* e)
{
    dynamic_cast<HUDButton*>(context)->onWin(e);
}
void HUDButtonLoseForwarder(Listener* context, const GameEvents::Event* e)
{
    dynamic_cast<HUDButton*>(context)->onLose(e);
}
