#ifndef HUDIMAGE_H
#define HUDIMAGE_H
#include "hudComponent.h"
#include "sprite.h"

class HUDImage : public HUDComponent {
public:
    HUDImage(const std::string& n, const Vector2f& p, bool v);
    HUDImage(const std::string& n, const Vector2f& p, bool v, const std::string& sp);
    HUDImage(const HUDImage& rhs);
    HUDImage& operator=(const HUDImage& rhs);
    virtual ~HUDImage() { delete sprite; }

    virtual void draw() const;
    void partialDraw(float dx, float dy) const;
    virtual void update(Uint32);

    Vector2f getSize() const;

private:
    Drawable* sprite;
};
#endif
