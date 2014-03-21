#ifndef TILE_H
#define TILE_H
#include <string>
#include "sprite.h"
#include "vector2f.h"

class Tile {
public:
    Tile() : sprite(), collidable(false) {};
    Tile(const std::string&);
    Tile(const std::string&, const Vector2f&);
    Tile(const std::string&, bool);
    Tile(const Tile& t);
    ~Tile() { }
    Tile& operator=(const Tile&);

    virtual void draw() const;
    virtual void update(Uint32& ticks) const;

private:
    Sprite *sprite;
    bool collidable;

};
#endif
