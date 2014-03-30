#ifndef TILE_H
#define TILE_H
#include <string>
#include "sprite.h"
#include "vector2f.h"
#include "particle.h"

class Tile {
public:
    Tile(const std::string&, const Vector2f&);
    Tile(const std::string&, const std::string&, const Vector2f&, const bool);
    Tile(const Tile& t);
    ~Tile(){ delete sprite; delete particleSys;} ;  
    Tile& operator=(const Tile&);

    void draw() const;
    void update(Uint32& ticks) const;
    const Vector2f& getCoord() const { return sprite->getPosition(); } 
    bool isCollidable() const { return collidable; }
    void setParticleSystem(ParticleSystem* ps) { delete particleSys; particleSys = ps; }
    const std::string& getId() const { return id; }
private:
    std::string id;
    Sprite *sprite;
    bool collidable;
    ParticleSystem* particleSys;
};
#endif
