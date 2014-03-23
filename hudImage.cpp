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
    sprite = new Sprite(*rhs.sprite);
    return *this;
}

void HUDImage::draw() const {
    if(isVisible())
    {
        sprite->draw();
    }
}

void HUDImage:: update(Uint32 ticks) {
    sprite->update(ticks);
}
