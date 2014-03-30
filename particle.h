#ifndef PARTICLE_H
#define PARTICLE_H
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <string>
#include "gamedata.h"
#include "vector2f.h"

class Particle {
public:
        Particle() :
            lifetime(),
            x(),
            y(),
            z(),
            r(),
            g(),
            b(),
            size(),
            angle(),
            vel(),
            startPos()
        {} 

        Particle(int nl, int nx, int ny, int nz, int nr, int ng, int nb, float s, float a, Vector2f v) :
            lifetime(nl),
            x(nx),
            y(ny),
            z(nz),
            r(nr),
            g(ng),
            b(nb),
            size(s),
            angle(a),
            vel(v),
            startPos()
        { }
        float lifetime;

        int x;
        int y;
        int z;

        int r;
        int g;
        int b;

        float size;
        float angle;
        Vector2f vel;
        Vector2f startPos;
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
