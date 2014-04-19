#ifndef MAP__H
#define MAP__H
#include <string>
#include <cmath>
#include "xmlparser.h"
#include "tile.h"
#include "gridElement.h"

class GridElement;

class MapManager : public Listener {
public:
    static MapManager& getInstance();
    void displayData() const;

    ~MapManager();

    //void load();
    void draw() const;
    void drawGridElements(int) const;
    void update(Uint32& ticks);

    void debug() const;

    void addGridElement(GridElement*);
    const Tile& findTileAt(const Vector2f&) const;


    void collideGridEles(int, GridElement&, Vector2f, Vector2f&, bool&, GridElement*& ) const;
    Vector2f validateMovement(GridElement&,  Vector2f, float&, bool&) const;

    int getW() const { return mapWidth;}
    int getH() const { return mapHeight;}
    int getTileWidth() const { return tileWidth; }
    int getTileHeight() const {return tileHeight; }
    int getTileRise() const { return tileRise; }
    int getGridTileWidth() const { return  sqrt(pow(tileWidth/2.0,2) + pow(tileHeight/2.0,2)) ;}
    int getGridTileHeight() const { return  sqrt(pow(tileWidth/2.0,2) + pow(tileHeight/2.0,2)) ;}
    int getNumGridElements() const {
        return numGridElements;
    }
    Vector2f getOrigin() const;
    Vector2f gridToWorld(Vector2f) const;
    Vector2f worldToGrid(Vector2f) const;
    int getIndexAt(const Vector2f&) const;

    GridElement* getPlayer() const { return player; }
    void setPlayer(GridElement* p) { player = p; }

private:
//    MapManager(const std::string& fn = "xmlSpec/basicMap.xml");
//    MapManager(const std::string& fn = "xmlSpec/testMap.xml");
    MapManager(const std::string& fn = "xmlSpec/test.xml");
//    MapManager(const std::string& fn = "xmlSpec/wuh.xml");
    MapManager(const MapManager&);
    MapManager& operator=(const MapManager&);

    void createTiles();
    void createLayers();

    XMLParser& parser;

    int numGridElements;
    GridElement* player;

    /* Storage Structures */


    std::map<std::string, std::string> tiles;
    std::list<Tile*> updateTiles;
    std::list<std::vector<Tile> > mapLayers;
    std::vector<std::list<GridElement*> > gridElements; // ordered by relevant tile

    /* Map Constants */

    unsigned int tileWidth; // these two in display coordinates
    unsigned int tileHeight;
    unsigned int tileRise;
    int mapWidth;
    int mapHeight;
    std::string weather;
    virtual void registerListeners();
};

void MapCreateForwarder(Listener*, const GameEvents::Event);
#endif
