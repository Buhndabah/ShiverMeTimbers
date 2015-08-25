#ifndef MAP__H
#define MAP__H
#include <string>
#include <cmath>
#include "xmlparser.h"
#include "tile.h"
#include "listener.h"
#include "gridElement.h"
#include "gameEvents.h"

class GridElement;

class MapManager : public Listener {
public:
    static MapManager& getInstance();

    ~MapManager();
    void reinit();

    void debug() const;

    void draw() const;
    void drawGridElements(int) const;
    void update(Uint32& ticks);

    void addGridElement(GridElement*);
    void removeGridElement(const std::string&);

    GridElement* getPlayer() const { return player; }
    void setPlayer(GridElement* p) { player = p; }

    void collideGridEles(int, GridElement*) const;
    void validateMovement(GridElement*) const;

    int getW() const { return mapWidth;}
    int getH() const { return mapHeight;}
    int getTileWidth() const { return tileWidth; }
    int getTileHeight() const {return tileHeight; }
    int getTileRise() const { return tileRise; }
    int getGridTileWidth() const { return  sqrt(pow(tileWidth/2.0,2) + pow(tileHeight/2.0,2)) ;}
    int getGridTileHeight() const { return  sqrt(pow(tileWidth/2.0,2) + pow(tileHeight/2.0,2)) ;}
    int getNumGridElements() const { return numGridElements; }
    Vector2f getOrigin() const { return origin; }

    int getIndexAt(const Vector2f&) const;
    const Tile& findTileAt(const Vector2f&) const;

    virtual void onDeath(const GameEvents::Event*);
    virtual void onCreate(const GameEvents::Event*);
    
private:
    MapManager(const std::string& fn = "xml/xmlSpec/416Final.xml");
    MapManager(const MapManager&);
    MapManager& operator=(const MapManager&);

    void createTiles();
    void createLayers();

    XMLParser& parser;

    int numGridElements;
    GridElement* player;

    Tile* dummyTile;

    /* Storage Structures */

    std::map<std::string, std::string> tiles;
    std::map<GridElement*, int> reserve;
    std::list<Tile*> updateTiles;
    std::list<std::vector<Tile> > mapLayers;
    std::vector<std::list<GridElement*> > gridElements; // ordered by relevant tile

    /* Map Constants */

    unsigned int tileWidth; // these two in display coordinates
    unsigned int tileHeight;
    unsigned int tileRise;
    int mapWidth;
    int mapHeight;
    Vector2f origin;
    std::string weather;
    virtual void registerListeners();
};

void MapCreateForwarder(Listener*, const GameEvents::Event);
void MapDeathForwarder(Listener*, const GameEvents::Event);
#endif
