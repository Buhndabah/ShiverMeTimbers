#include <cmath>
#include "ioManager.h"
#include "viewport.h"
#include "particle.h"

void AbstractParticle::draw() {
    SDL_FillRect(IOManager::getInstance().getScreen(), &borderRect, borderColor);
    SDL_FillRect(IOManager::getInstance().getScreen(), &rect, color);
}

SnowBehavior::SnowBehavior(const Vector2f& pos, const Vector2f& dim, int viewW, int viewH, int maxH, int maxL) :
    basePos(pos),
    maxDim(dim),
    viewWidth(viewW),
    viewHeight(viewH),
    maxHeight(maxH),
    maxLife(maxL)
{ }

// Update for snow particles
void SnowBehavior::operator()(Uint32 ticks, Particle<SnowBehavior>* p) const {
    Vector2f incr;
    int startX;
    int startY;
    float seconds = static_cast<float>(ticks)/1000.0;

    // check if we're fully initialized yet
    if(!p->isInit())
    {
        int xOffset = rand() % static_cast<int>(maxDim[0]);
        int yOffset = rand() % static_cast<int>(maxDim[1]);
        p->setX(p->getX() + xOffset);
        p->setY(p->getY() + yOffset);

        p->setLife(rand()%maxLife + 5);
        p->setSize(rand() % 4 + 3);
        p->setAngle(1);
        p->setVel(Vector2f(0,50));

        p->setR(220);
        p->setG(220);
        p->setB(220);
        p->setR( (p->getStartPos()[1] + p->getR()) > 255 ? 255 : (p->getStartPos()[1] + p->getR()));
        p->setG( (p->getStartPos()[1] + p->getG()) > 255 ? 255 : (p->getStartPos()[1] + p->getG()));
        p->setB( (p->getStartPos()[1] + p->getB()) > 255 ? 255 : (p->getStartPos()[1] + p->getB()));

        p->setBorderColor(SDL_MapRGB(IOManager::getInstance().getScreen()->format, 0,0,0));
        p->setColor(SDL_MapRGB(IOManager::getInstance().getScreen()->format, p->getR(), p->getG(), p->getB()));

        p->setInit();
    }
    else 
    {
        // first decrement life
        p->setLife(p->getLife() - seconds);

        // time to reset
        if( (p->getLife() <= 0) || (p->getZ() <= p->getStartPos()[1]) ) {

            p->setLife(rand()%maxLife + 5);

            startX = rand() % static_cast<int>(maxDim[0]);
            startY = rand() % static_cast<int>(maxDim[1]);

            p->setX(basePos[0] + startX);
            p->setY(basePos[1] + startY);
            p->setZ(rand() % maxHeight + 3*maxHeight/4);

            p->setStartPos(Vector2f(startX, startY));

            p->setR(220);
            p->setG(220);
            p->setB(220);

            p->setR( (p->getStartPos()[1] + p->getR()) > 255 ? 255 : (p->getStartPos()[1] + p->getR()));
            p->setG( (p->getStartPos()[1] + p->getG()) > 255 ? 255 : (p->getStartPos()[1] + p->getG()));
            p->setB( (p->getStartPos()[1] + p->getB()) > 255 ? 255 : (p->getStartPos()[1] + p->getB()));
        }
        // just move
        else 
        {
            incr = p->getVel()*seconds;
            p->setX(p->getX() + incr[0]);
            p->setZ(p->getZ() - incr[1]);
        }
    }
    p->setBorderX(p->getX() - Viewport::getInstance().X()-2);
    p->setBorderY(p->getY()-p->getZ()-Viewport::getInstance().Y()-2);
    p->setBorderW((int)p->getSize() +4);
    p->setBorderH((int)p->getSize() +4);
    p->setRectX(p->getX() - Viewport::getInstance().X());
    p->setRectY(p->getY()-p->getZ()-Viewport::getInstance().Y());
    p->setRectW((int)p->getSize());
    p->setRectH((int)p->getSize());
}

// Currently these are all values specific to "snow" particles
ParticleSystem::ParticleSystem(const Vector2f& p, const Vector2f& d, int h, const std::string& t) : 
    pos(p),
    dim(d),
    viewWidth(Gamedata::getInstance().getXmlInt("viewWidth")),
    viewHeight(Gamedata::getInstance().getXmlInt("viewHeight")),
    maxHeight(h),
    maxLife(30),
    maxCount(10),
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
    particles.clear();
    spawnParticles(type);
}

ParticleSystem& ParticleSystem::operator=(const ParticleSystem& rhs) {
    if(this == &rhs)
        return *this;

    pos = rhs.pos;
    dim = rhs.dim;
    viewWidth = rhs.viewWidth;
    viewHeight = rhs.viewHeight;
    maxHeight = rhs.maxHeight;
    maxLife = rhs.maxLife;
    maxCount = rhs.maxCount;
    type = rhs.type;

    particles.clear();
    spawnParticles(type);
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

    // Iterate through particles
    for(std::list<AbstractParticle*>::const_iterator it=particles.begin(); it!= particles.end(); ++it)
    {
        (*it)->draw();
    }
}

// Currently snow specific
void ParticleSystem::update(Uint32 ticks) {
    std::list<AbstractParticle*>::iterator it = particles.begin();
    
    while(it!=particles.end())
    {
        (*it)->update(ticks);
        ++it;
    }
}

// fill container with maxCount particles
void ParticleSystem::spawnParticles(const std::string& type) {
    AbstractParticle *p;
    Vector2f newPos;

    while(particles.size() < maxCount)
    {
        // Random offset

        if(type.compare("snow")==0)
        {
            SnowBehavior snowB(pos, dim, viewWidth, viewHeight, maxHeight, maxLife);
            p = new Particle<SnowBehavior>( pos[0],
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
