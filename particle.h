#ifndef PARTICLE_H
#define PARTICLE_H
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <string>
#include "gamedata.h"
#include "vector2f.h"

/* This class stores constants used to create the actual particles, contains no actual methods */
class Particle {
public:
        Particle() :
            lifetime(),
            x(),
            y(),
            z(),
            startPos(),
            r(),
            g(),
            b(),
            size(),
            angle(),
            vel()
        { } 

        Particle(int nl, int nx, int ny, int nz, int nr, int ng, int nb, float s, float a, Vector2f v) :
            lifetime(nl),
            x(nx),
            y(ny),
            z(nz),
            startPos(),
            r(nr),
            g(ng),
            b(nb),
            size(s),
            angle(a),
            vel(v)
        { }

        Particle(const Particle& rhs) : 
            lifetime(rhs.lifetime),
            x(rhs.x),
            y(rhs.y),
            z(rhs.z),
            startPos(rhs.startPos),
            r(rhs.r),
            g(rhs.g),
            b(rhs.b),
            size(rhs.size),
            angle(rhs.angle),
            vel(rhs.vel)
        { }

        Particle& operator=(const Particle& rhs) {
            if(this == &rhs) return *this;

            lifetime = rhs.lifetime;
            x = rhs.x;
            y = rhs.y;
            z = rhs.z;
            startPos = rhs.startPos;
            r = rhs.r;
            g = rhs.g;
            b = rhs.b;
            size = rhs.size;
            angle = rhs.angle;
            vel = rhs.vel;

            return *this;
        }

        /* Data begins here */
    
        float lifetime;

        /* Position values */

        int x;
        int y;
        int z;
        Vector2f startPos;

        /* Color values */

        int r;
        int g;
        int b;

        float size;
        float angle;
        Vector2f vel;
};

class ParticleSystem {
public:
    ParticleSystem();
    ParticleSystem(const Vector2f&, const Vector2f&, int);
    ParticleSystem(const ParticleSystem&);
    ParticleSystem& operator=(const ParticleSystem&);
    ~ParticleSystem();

    void draw() const;
    void update(Uint32);
private:
  
    void spawnParticles();
   
    Vector2f pos;
    Vector2f dim;

    int viewWidth;
    int viewHeight;

    int maxHeight;
    int maxLifeTime;
    unsigned int maxCount;

    std::list<Particle*> particles;
};
#endif
