#include <cmath>
#include "gamedata.h"
#include "particleSystem.h"

// Maxlife and maxCount values set with snow in mind, might need to change later
ParticleSystem::ParticleSystem(const Vector2f& p, const Vector2f& d, int h, const std::string& t) :
    pos(p),
    dim(d),
    viewWidth(Gamedata::getInstance().getXmlInt("viewWidth")),
    viewHeight(Gamedata::getInstance().getXmlInt("viewHeight")),
    maxHeight(h),
    maxLife(30),
    maxCount(7),
    particles(),
    type(t)
{
    spawnParticles(type);
}

ParticleSystem::ParticleSystem(const ParticleSystem& rhs) :
    pos(rhs.pos),
    dim(rhs.dim),
    viewWidth(rhs.viewWidth),
    viewHeight(rhs.viewHeight),
    maxHeight(rhs.maxHeight),
    maxLife(rhs.maxLife),
    maxCount(rhs.maxCount),
    particles(),
    type(rhs.type)
{
    while(!particles.empty())
    {
        delete particles.front();
        particles.erase(particles.begin());
    }
    spawnParticles(type);
}

ParticleSystem& ParticleSystem::operator=(const ParticleSystem& rhs) {
    if(this == &rhs) return *this;

    pos = rhs.pos;
    dim = rhs.dim;
    viewWidth = rhs.viewWidth;
    viewHeight = rhs.viewHeight;
    maxHeight = rhs.maxHeight;
    maxLife = rhs.maxLife;
    maxCount = rhs.maxCount;
    type = rhs.type;


    std::list<AbstractParticle*>::iterator it = particles.begin();
    while(!particles.empty())
    {
        delete (*it);
        particles.erase(it);
    }
    spawnParticles(type);

    return *this;
}

ParticleSystem::~ParticleSystem() {
    while(!particles.empty()) {
        delete particles.front();
        particles.erase(particles.begin());
    }
}

void ParticleSystem::draw() const {
    for(std::list<AbstractParticle*>::const_iterator it = particles.begin(); it!= particles.end(); ++it)
    {
        (*it)->draw();
    }
}

void ParticleSystem::update(Uint32 ticks) {
    for(std::list<AbstractParticle*>::const_iterator it = particles.begin(); it!= particles.end(); ++it)
    {
        (*it)->update(ticks);
    }
}

// fill container with maxCount particles
void ParticleSystem::spawnParticles(const std::string& type) {
    AbstractParticle *p;

    while(particles.size() < maxCount)
    {
        if(type.compare("snow")==0)
        {
            SnowBehavior snowB(pos, dim, viewWidth, viewHeight, maxHeight, maxLife);
            p = new Particle<SnowBehavior>(pos[0],
                                           pos[1],
                                           rand() % maxHeight + maxHeight/4,
                                           snowB
                                           );
        }
        else
        {
            throw std::string("Unrecognized type for particle \"")+type+std::string("\"");
        }
        p->setStartPos(Vector2f(p->getX(),p->getY()));
        particles.push_back(p);
    }
}

