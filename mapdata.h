#ifndef MAP__H
#define MAP__H
#include <string>
#include "xmlparser.h"
#include "tile.h"

class Mapdata {
public:
    static Mapdata& getInstance();
    void displayData() const;

    //void load();
    void draw() const;
    void update(Uint32& ticks);

    void debug() const;

private:
    XMLParser parser;
    Mapdata(const std::string& fn = "xmlSpec/testMap.xml");
    Mapdata(const Mapdata&);
    Mapdata& operator=(const Mapdata&);

    void createTiles();
    void createLayers();

    std::map<unsigned int, std::string> tiles;
    std::list<std::list<Tile> > mapLayers;

    unsigned int tileWidth;
    unsigned int tileHeight;
    unsigned int mapWidth;
    unsigned int mapHeight;
};
#endif
