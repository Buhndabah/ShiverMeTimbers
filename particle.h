#ifndef PARTICLE_H
#define PARTICLE_H
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <string>
#include "gamedata.h"
#include "vector2f.h"


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
            angle()
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
            angle(rhs.angle)
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

        return *this;
    }
    
    virtual ~AbstractParticle() {}

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
};

/* This class stores constants used to create the actual particles, contains no actual methods */
template <class particleT>
class Particle : public AbstractParticle{
friend class ParticleSystem;
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

class ParticleSystem {
public:
    ParticleSystem(const Vector2f&, const Vector2f&, int, const std::string&);
    ParticleSystem(const ParticleSystem&);
    ParticleSystem& operator=(const ParticleSystem&);
    ~ParticleSystem();

    void draw() const;
    void update(Uint32);
private:
    Vector2f pos;
    Vector2f dim;

    int viewWidth;
    int viewHeight;
    int maxHeight;
    int maxLife;
    unsigned int maxCount;
    std::list<AbstractParticle *> particles;
    void spawnParticles(const std::string&);
    std::string type;
};
#endif
