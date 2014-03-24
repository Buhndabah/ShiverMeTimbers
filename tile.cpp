#include "tile.h"

Tile::Tile(const std::string& s, const Vector2f& v) :
    sprite(new Sprite(s,v, Vector2f(0,0))),
    collidable(false)
{ }

Tile::Tile(const std::string& s, const Vector2f&v, const bool b) :
    sprite(new Sprite(s,v, Vector2f(0,0))),
    collidable(b)
{ }

Tile::Tile(const Tile& t) :
    sprite(t.sprite),
    collidable(t.collidable)
{ }

Tile& Tile::operator=(const Tile& rhs) {
    sprite = new Sprite(*(rhs.sprite));
    collidable = rhs.collidable;
    return *this;
}

void Tile::draw() const {
    sprite->draw();
}

void Tile::update(Uint32& ticks) const {
    sprite->update(ticks);
}
