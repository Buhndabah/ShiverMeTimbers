#ifndef MAP__H
#define MAP__H
#include <string>
#include <cmath>
#include "xmlparser.h"
#include "tile.h"
#include "gridElement.h"

class GridElement;

class MapManager {
public:
    static MapManager& getInstance();
    void displayData() const;

    ~MapManager() {}

    //void load();
    void draw() const;
    void update(Uint32& ticks);

    void debug() const;

    int getW() const { return mapWidth;}
    int getH() const { return mapHeight;}
    int getTileWidth() const { return  sqrt(pow(tileWidth/2.0,2) + pow(tileHeight/2.0,2)) ;}
    int getTileHeight() const { return  sqrt(pow(tileWidth/2.0,2) + pow(tileHeight/2.0,2)) ;}

    Vector2f getOrigin() const;
    const Tile& findTileAt(const Vector2f&) const;
    //Vector2f validateMovement(Vector2f, Vector2f, int, float&, float, bool&) const;
    Vector2f validateMovement(GridElement&, Vector2f, float&, bool&) const;
private:
    XMLParser parser;
    MapManager(const std::string& fn = "xmlSpec/testMap.xml");
    MapManager(const MapManager&);
    MapManager& operator=(const MapManager&);

    void createTiles();
    void createLayers();

    std::map<std::string, std::string> tiles;
    std::list<std::vector<Tile> > mapLayers;

    // in display coord
    unsigned int tileWidth;
    unsigned int tileHeight;
    int mapWidth;
    int mapHeight;
};
#endif
