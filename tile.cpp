#include "tile.h"

Tile::Tile(const std::string& s, const Vector2f& v) :
    id("0"),
    sprite(new Sprite(s,v, Vector2f(0,0))),
    collidable(false),
    particleSys(NULL)
{ }

Tile::Tile(const std::string& i, const std::string& s, const Vector2f&v, const bool b) :
    id(i),
    sprite(new Sprite(s,v, Vector2f(0,0))),
    collidable(b),
    particleSys(NULL)
{ }

Tile::Tile(const Tile& t) :
    id(t.id),
    sprite(new Sprite(*(t.sprite))),
    collidable(t.collidable),
    particleSys(NULL)
{ 
    if(t.particleSys) {
        particleSys = new ParticleSystem(*(t.particleSys));
    }
}

Tile& Tile::operator=(const Tile& rhs) {
    if(this == &rhs) return *this;
    
    id=rhs.id;

    delete sprite;
    sprite = new Sprite(*(rhs.sprite));
    collidable = rhs.collidable;
    if(rhs.particleSys) {
        particleSys = new ParticleSystem(*(rhs.particleSys));
    }
    return *this;
}

void Tile::draw() const {
    sprite->draw();
    if(particleSys)
        particleSys->draw();
}

void Tile::update(Uint32& ticks) const {
    sprite->update(ticks);
    if(particleSys) {
        particleSys->update(ticks);
    }
}

void Tile::addParticleSystem(int height, const std::string& type) {
    if(particleSys) {
        delete particleSys;
    }
    particleSys = new ParticleSystem(getCoord(), getDim(), sprite->Y() + height, type);
}

bool Tile::pointOn(const Vector2f& coord) const {
    if( !coord[0] >= sprite->getPosition()[0] &&
        !coord[0] <= sprite->getPosition()[0] + sprite->getW() &&
        !coord[1] >= sprite->getPosition()[1] &&
        !coord[1] <= sprite->getPosition()[1] + sprite->getH()
      )
    {
        return false;
    }
    else return true;
}
