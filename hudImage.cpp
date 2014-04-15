#include "viewport.h"
#include "hudImage.h"

HUDImage::HUDImage(const std::string& n, const Vector2f& p, bool v) :
    HUDComponent(n,p,v),
    sprite()
{ 
}

HUDImage::HUDImage(const std::string& n, const Vector2f& p, bool v,  const std::string& sp) :
    HUDComponent(n,p,v),
    sprite(new Sprite(sp,getPosition(),Vector2f(0,0)))
{ 
}

HUDImage& HUDImage::operator=(const HUDImage& rhs)
{
    HUDComponent::operator=(rhs);
    delete sprite;
    sprite = new Sprite((Sprite&)*rhs.sprite);
    return *this;
}

void HUDImage::draw() const {
    if(isVisible())
    {
        sprite->draw();
    }
}

void HUDImage::scaledDraw(float dx, float dy) const {
    if(isVisible())
    {
        sprite->scaledDraw(dx, dy);
    }
}

void HUDImage::update(Uint32 ticks) {
    HUDComponent::update(ticks);

    if(hasFlicker()) 
    {
        if(getElapsedTicks()/1000 == 0)
        {
            //
        }
        else if( (getElapsedTicks()/1000) % timeToFlicker()==0.0) 
        {
            setVisible(!isVisible());
            resetTicks();
        }
    }
    sprite->update(ticks);
    sprite->setPosition(Vector2f(Viewport::getInstance().X()+getPosition()[0], Viewport::getInstance().Y()+getPosition()[1]));
}

Vector2f HUDImage::getSize() const{ 
    return sprite->getSize(); 
}
