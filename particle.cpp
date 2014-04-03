#include <cmath>
#include "ioManager.h"
#include "viewport.h"
#include "particle.h"

// Default constructor creates a system that does nothing, for now
ParticleSystem::ParticleSystem() :
    pos(Vector2f(0,0)),
    dim(Vector2f(0,0)),
    viewWidth(Gamedata::getInstance().getXmlInt("viewWidth")),
    viewHeight(Gamedata::getInstance().getXmlInt("viewHeight")),
    maxHeight(0),
    maxLifeTime(0),
    maxCount(0),
    particles(0,NULL)
{ }

// Currently these are all values specific to "snow" particles
ParticleSystem::ParticleSystem(const Vector2f& p, const Vector2f& d, int h) : 
    pos(p),
    dim(d),
    viewWidth(Gamedata::getInstance().getXmlInt("viewWidth")),
    viewHeight(Gamedata::getInstance().getXmlInt("viewHeight")),
    maxHeight(h),
    maxLifeTime(30),
    maxCount(10),
    particles()
{ 
    spawnParticles();
    }

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

// Currently this is snow specific
void ParticleSystem::draw() const {
    SDL_Rect rect;
    Uint32 color;
    int borderSize;

    // Iterate through particles
    for(std::list<Particle*>::const_iterator it=particles.begin(); it!= particles.end(); ++it)
    {

        /* Draws a grey-white rect surrounded by a black border */
  
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

// Currently snow specific
void ParticleSystem::update(Uint32 ticks) {
    Vector2f incr;
    std::list<Particle*>::iterator it = particles.begin();
    Particle* p;
    int startX;
    int startY;

    while(it!=particles.end())
    {
            p=*it;

            // Decrement this particle's life
            p->lifetime-=static_cast<float>(ticks)/1000.0;

            // Reinitialize particles if they've expired XXX TODO THIS IS BAD READ IN FROM SOMEWHERE ELSE PLEASE
            if(p->lifetime<=0 
                || (p->z <= p->startPos[1]))
            {
                p->lifetime=rand()%maxLifeTime + 5;
                startX = rand() % static_cast<int>(dim[0]);
                startY= rand() % static_cast<int>(dim[1]);
                p->x=pos[0]+startX;
                p->y=pos[1] + startY;
                p->z=rand() % maxHeight + 3*maxHeight/4,
                p->startPos=Vector2f(startX,startY);
                p->r= p->startPos[1]+p->r > 255 ?  255 : p-> startPos[1] + p->r; 
                p->g= p->startPos[1]+p->g > 255 ?  255 : p-> startPos[1] + p->g; 
                p->b= p->startPos[1]+p->b > 255 ?  255 : p-> startPos[1] + p->b; 
                 
            }
            else
            {
                // increment by velocity
                incr = p->vel*static_cast<float>(ticks)/1000.0;
                p->x+=incr[0];
                p->y;
                p->z-=incr[1];
            }
                ++it;
    }
}

// fill container with maxCount particles
void ParticleSystem::spawnParticles() {
    Particle *p;
    Vector2f newPos;
    int xOffset;
    int yOffset;

    while(particles.size() < maxCount)
    {
        // Random offset
        xOffset = rand() % static_cast<int>(dim[0]);
        yOffset = rand() % static_cast<int>(dim[1]);

        p = new Particle(rand() % maxLifeTime + 5,
                         pos[0] + xOffset,
                         pos[1]  + yOffset,
                         rand() % maxHeight + maxHeight/4,
                         220,
                         220,
                         220,
                         rand() % 4 + 3,
                         1,
                         Vector2f(0,50)
                );
        p->startPos=Vector2f(p->x,p->y);
        p->r= p->startPos[1]+p->r > 255 ?  255 : p-> startPos[1] + p->r; 
        p->g= p->startPos[1]+p->g > 255 ?  255 : p-> startPos[1] + p->g; 
        p->b= p->startPos[1]+p->b > 255 ?  255 : p-> startPos[1] + p->b; 
        particles.push_back(p);

    }
}
