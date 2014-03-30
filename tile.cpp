#include "tile.h"

Tile::Tile(const std::string& s, const Vector2f& v) :
    sprite(new Sprite(s,v, Vector2f(0,0))),
    collidable(false),
    particleSys(new ParticleSystem())
{ }

Tile::Tile(const std::string& s, const Vector2f&v, const bool b) :
    sprite(new Sprite(s,v, Vector2f(0,0))),
    collidable(b),
    particleSys(new ParticleSystem())
{ }

Tile::Tile(const Tile& t) :
    sprite(new Sprite(*(t.sprite))),
    collidable(t.collidable),
    particleSys(new ParticleSystem(*(t.particleSys)))
{ }

Tile& Tile::operator=(const Tile& rhs) {
    if(this == &rhs) return *this;
    
    delete sprite;
    sprite = new Sprite(*(rhs.sprite));
    collidable = rhs.collidable;
    particleSys = new ParticleSystem(*(rhs.particleSys));

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
