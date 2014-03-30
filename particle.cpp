#include <cmath>
#include "ioManager.h"
#include "viewport.h"
#include "particle.h"

ParticleSystem::ParticleSystem() :
    pos(Vector2f(0,0)),
    dim(Vector2f(0,0)),
    viewWidth(Gamedata::getInstance().getXmlInt("viewWidth")),
    viewHeight(Gamedata::getInstance().getXmlInt("viewHeight")),
    maxHeight(0),
    maxLifeTime(0),
    maxCount(0),
    particles(0,NULL)
{
}

ParticleSystem::ParticleSystem(const Vector2f& p, const Vector2f& d, int h) : 
    pos(p),
    dim(d),
    viewWidth(Gamedata::getInstance().getXmlInt("viewWidth")),
    viewHeight(Gamedata::getInstance().getXmlInt("viewHeight")),
    maxHeight(h),
    maxLifeTime(30),
    maxCount(3),
    particles()
{ }

ParticleSystem::ParticleSystem(const ParticleSystem& rhs) :
    pos(rhs.pos),
    dim(rhs.dim),
    viewWidth(rhs.viewWidth),
    viewHeight(rhs.viewHeight),
    maxHeight(rhs.maxHeight),
    maxLifeTime(rhs.maxLifeTime),
    maxCount(rhs.maxCount),
    particles()
{
    particles.clear();
    for(std::list<Particle*>::const_iterator it=rhs.particles.begin(); it!=rhs.particles.end(); ++it)
    {
        particles.push_back(new Particle(*(*it)));
    }
}

ParticleSystem& ParticleSystem::operator=(const ParticleSystem& rhs) {
    if(this == &rhs)
        return *this;

    pos = rhs.pos;
    dim = rhs.dim;
    viewWidth = rhs.viewWidth;
    viewHeight = rhs.viewHeight;
    maxHeight = rhs.maxHeight;
    maxLifeTime = rhs.maxLifeTime;
    maxCount = rhs.maxCount;

    particles.clear();
    particles = std::list<Particle*>(maxCount);

    for(std::list<Particle*>::const_iterator it=rhs.particles.begin(); it!=rhs.particles.end(); ++it)
    {
        particles.push_back(new Particle(*(*it)));
    }
    return *this;
}

ParticleSystem::~ParticleSystem() {
    while(!particles.empty()) {
        delete particles.front();
        particles.erase(particles.begin());
    }
}

void ParticleSystem::draw() const {
    SDL_Rect rect;
    Uint32 color;
    int borderSize;
    for(std::list<Particle*>::const_iterator it=particles.begin(); it!= particles.end(); ++it)
    {
        borderSize = 2;
        //borderSize = (*it)->size/4 > 0 ? (*it)->size/3 : 1;

        rect.x = (*it)->x-Viewport::getInstance().X()- borderSize;
        rect.y = (*it)->y-(*it)->z-Viewport::getInstance().Y()-borderSize;
        rect.w = (int)(*it)->size+ (borderSize*2);
        rect.h = (int)(*it)->size+ (borderSize*2);

        color = SDL_MapRGB(IOManager::getInstance().getScreen()->format, 0,0,0);
        SDL_FillRect(IOManager::getInstance().getScreen(), &rect, color);


        rect.x = (*it)->x-Viewport::getInstance().X();
        rect.y = (*it)->y-(*it)->z-Viewport::getInstance().Y();
        rect.w = (int)(*it)->size;
        rect.h = (int)(*it)->size;

        color = SDL_MapRGB(IOManager::getInstance().getScreen()->format, (*it)->r,(*it)->g,(*it)->b);
        SDL_FillRect(IOManager::getInstance().getScreen(), &rect, color);
    }
}

void ParticleSystem::update(Uint32 ticks) {
    Vector2f incr;
    std::list<Particle*>::iterator it = particles.begin();
    while(it!=particles.end())
    {
            
            (*it)->lifetime-=static_cast<float>(ticks)/1000.0;
            if((*it)->lifetime<=0 
                || ((*it)->z <= (*it)->startPos[1]))
            {
                particles.erase(it++);
            }
            else
            {
                incr = (*it)->vel*static_cast<float>(ticks)/1000.0;
                (*it)->x+=incr[0];
                (*it)->y;
                (*it)->z-=incr[1];
                it++;
            }
    }
    spawnParticles();
}

void ParticleSystem::spawnParticles() {
    Particle *p;
    Vector2f newPos;
    int startX;
    int startY;
    while(particles.size() < maxCount)
    {
        startX = rand() % static_cast<int>(dim[0]);
        startY= rand() % static_cast<int>(dim[1]);

        p = new Particle(rand() % maxLifeTime + 5,
                         pos[0] + startX,
                         pos[1]  + startY,
                         rand() % maxHeight + 2*maxHeight/3,
                         220,
                         220,
                         220,
                         rand() % 4 + 3,
                         1,
                         Vector2f(0,50)
                );
        p->startPos=Vector2f(startX,startY);
        p->r= p->startPos[1]+p->r > 255 ?  255 : p-> startPos[1] + p->r; 
        p->g= p->startPos[1]+p->g > 255 ?  255 : p-> startPos[1] + p->g; 
        p->b= p->startPos[1]+p->b > 255 ?  255 : p-> startPos[1] + p->b; 
        particles.push_back(p);
    }
}
