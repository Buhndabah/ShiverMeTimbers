#ifndef TILE_H
#define TILE_H
#include <string>
#include "sprite.h"
#include "vector2f.h"
#include "particleSystem.h"

class Tile {
public:
    Tile();
    Tile(const std::string&, const Vector2f&);
    Tile(const std::string&, const std::string&, const std::string&, const Vector2f&, const bool);
    Tile(const Tile& t);
    ~Tile(){ delete sprite; delete particleSys;} ;  
    Tile& operator=(const Tile&);

    void draw() const;
    void update(Uint32& ticks) const;
    const Vector2f& getCoord() const { return sprite->getPosition(); } 
    const Vector2f getDim() const { return sprite->getSize(); }
    bool isCollidable() const { return collidable; }
    void addParticleSystem(int, const std::string&);
    const std::string& getId() const { return id; }
    const std::string& getName() const { return name; }
    bool pointOn(const Vector2f&) const;
private:
    std::string name;
    std::string id;
    Sprite *sprite;
    bool collidable;
    ParticleSystem* particleSys;
};
#endif
