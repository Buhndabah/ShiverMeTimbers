#ifndef PARTICLE_H
#define PARTICLE_H
#include <SDL.h>
#include <SDL_image.h>
#include "vector2f.h"

// ABC for particles, ParticleSystem stores a list of these
class AbstractParticle {
public:
    AbstractParticle(int posX, int posY, int posZ) :
            init(false),
            lifetime(),
            x(posX),
            y(posY),
            z(posZ),
            startPos(),
            vel(),
            r(),
            g(),
            b(),
            size(),
            angle(),
            rect(),
            borderRect(),
            color(),
            borderColor()
    { }

    AbstractParticle(const AbstractParticle& rhs) :
            init(rhs.init),
            lifetime(rhs.lifetime),
            x(rhs.x),
            y(rhs.y),
            z(rhs.z),
            startPos(rhs.startPos),
            vel(rhs.vel),
            r(rhs.r),
            g(rhs.g),
            b(rhs.b),
            size(rhs.size),
            angle(rhs.angle),
            rect(rhs.rect),
            borderRect(rhs.borderRect),
            color(rhs.color),
            borderColor(rhs.borderColor)
    { }

    AbstractParticle& operator=(const AbstractParticle& rhs) {
        if(this == &rhs) return *this;

        init = rhs.init;
        lifetime = rhs.lifetime;
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
        startPos = rhs.startPos;
        vel = rhs.vel;
        r = rhs.r;
        g = rhs.g;
        b = rhs.b;
        size = rhs.size;
        angle = rhs.angle;
        rect = rhs.rect;
        borderRect = rhs.borderRect;
        color=rhs.color;
        borderColor = rhs.borderColor;

        return *this;
    }
    
    virtual ~AbstractParticle() {}

    void draw();
    virtual void update(Uint32 ticks) =0;

    /* Getters and Setters */

    bool isInit() const { return init; }
    void setInit() { init = true; }

    float getLife() const { return lifetime; }
    void setLife(float seconds) { lifetime = seconds; }

    int getX() const { return x; }
    int getY() const { return y; }
    int getZ() const { return z; }

    void setX(int value) { x = value; }
    void setY(int value) { y = value; }
    void setZ(int value) { z = value; }

    Vector2f getStartPos() const { return startPos; }
    Vector2f getVel()      const { return vel; }

    void setStartPos(const Vector2f pos) { startPos = pos; }
    void setVel(const Vector2f speed) { vel = speed; }

    int getR() const { return r; }
    int getG() const { return g; }
    int getB() const { return b; }

    void setR(int red)   { r = red; }
    void setG(int green) { g = green; }
    void setB(int blue)  { b = blue; }

    float getSize()  const { return size; }
    float getAngle() const { return angle; }

    void setSize(float value)    { size = value; }
    void setAngle(float degrees) { angle = degrees; }

    SDL_Rect getRect() const { return rect; }
    SDL_Rect getBorderRect() const { return borderRect; }

    void setRectX(int x) { rect.x = x; }
    void setRectY(int y) { rect.y = y; }
    void setRectW(int w) { rect.w = w; }
    void setRectH(int h) { rect.h = h; }

    void setBorderX(int x) { borderRect.x = x; }
    void setBorderY(int y) { borderRect.y = y; }
    void setBorderW(int w) { borderRect.w = w; }
    void setBorderH(int h) { borderRect.h = h; }

    Uint32 getColor() const { return color; }
    Uint32 getBorderColor() const { return borderColor; } 

    void setColor(Uint32 c) { color = c; }
    void setBorderColor(Uint32 c) { borderColor = c; }
private:
    /* Data begins here */
   
    bool init;
    
    float lifetime;

    /* Position values */

    int x;
    int y;
    int z;
    Vector2f startPos;
    Vector2f vel;

    /* Color values */

    int r;
    int g;
    int b;

    float size;
    float angle;

    SDL_Rect rect;
    SDL_Rect borderRect;

    Uint32 color;
    Uint32 borderColor;
};

/* Templated, concrete implementation of AbstractParticle */
template <class particleT>
class Particle : public AbstractParticle{
public:
        Particle(int posX, int posY, int posZ, particleT behavior) :
            AbstractParticle(posX,posY,posZ),
            updateBehavior(behavior)
        { }

        Particle(const Particle& rhs) : 
            updateBehavior(rhs.updateBehavior)
        { }

        Particle& operator=(const Particle& rhs) {

            if(this == &rhs) return *this;

            AbstractParticle::operator=(rhs);
            updateBehavior = rhs.updateBehavior;

            return *this;
        }

        void update(Uint32 ticks) { updateBehavior(ticks, this); }

private:
        const particleT updateBehavior;
};


// update method for snow particles
class SnowBehavior {
public:
    SnowBehavior(const Vector2f&, const Vector2f&, int, int, int, int);
    void operator()(Uint32 ticks, Particle<SnowBehavior> * particle) const;

private:
    Vector2f basePos;
    Vector2f maxDim;
    int viewWidth;
    int viewHeight;
    int maxHeight;
    int maxLife;
};
#endif
