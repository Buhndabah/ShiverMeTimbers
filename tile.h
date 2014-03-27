#ifndef TILE_H
#define TILE_H
#include <string>
#include "sprite.h"
#include "vector2f.h"

class Tile {
public:
    Tile(const std::string&, const Vector2f&);
    Tile(const std::string&, const Vector2f&, const bool);
    Tile(const Tile& t);
    ~Tile(){ delete sprite; } ;  
    Tile& operator=(const Tile&);

    void draw() const;
    void update(Uint32& ticks) const;
    const Vector2f& getCoord() const { return sprite->getPosition(); } 
    bool isCollidable() const { return collidable; }
private:
    Sprite *sprite;
    bool collidable;
};
#endif
