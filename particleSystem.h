#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H
#include <string>
#include <list>
#include "particle.h"
#include "vector2f.h"

class ParticleSystem {
public:
    ParticleSystem(const Vector2f&, const Vector2f&, int, const std::string&);
    ParticleSystem(const ParticleSystem&);
    ParticleSystem& operator=(const ParticleSystem&);
    ~ParticleSystem();

    void draw() const;
    void update(Uint32);

private:
    void spawnParticles(const std::string&);

    Vector2f pos;
    Vector2f dim;

    int viewWidth;
    int viewHeight;
    
    int maxHeight;
    int maxLife;
    unsigned int maxCount;
    
    std::list<AbstractParticle* > particles;
    std::string type;
};
#endif
