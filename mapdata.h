#ifndef MAP__H
#define MAP__H
#include <string>
#include "xmlparser.h"

class Mapdata {
public:
    static Mapdata& getInstance();
    void displayData() const;

    void load();
    void draw() const;
    void update(Uint32 ticks);

    bool getXmlBool(const std::string&) const;
    const std::string& getXmlStr(const std::string&) const;
    float getXmlFloat(const std::string&) const;
    int getXmlInt(const std::string&) const;

    
private:
    Xmldata xmlData;
    Mapdata(const std::string& fn = "xmlSpec/example.tmx");
    Mapdata(const Mapdata&);
    Mapdata& operator=(const Mapdata&);

    Tile* mapTiles;
    unsigned int tileWidth;
    unsigned int tileHeight;
    unsigned int mapWidth;
    unsigned int mapHeight;
};
#endif
