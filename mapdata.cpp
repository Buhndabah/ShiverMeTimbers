#include <algorithm>
#include <sstream>
#include <iostream>
#include "vector2f.h"
#include "gamedata.h"
#include "mapdata.h"

Mapdata& Mapdata::getInstance() {
    static Mapdata instance;
    return instance;
}

Mapdata::Mapdata(const std::string& fn) :
    parser(fn),
    tiles(),
    mapLayers(),
    tileWidth(),
    tileHeight(),
    mapWidth(),
    mapHeight()
{
    std::stringstream strm; 
    std::map<std::string, std::string> rootData = parser.parseNode(parser.findNodes("map").front());
    strm<<rootData[std::string("tilewidth")];
    strm >> tileWidth;
    strm.clear();
    strm<<rootData[std::string("tileheight")];
    strm >> tileHeight;
    strm.clear();
    strm << rootData[std::string("width")];
    strm >> mapWidth;
    strm.clear();
    strm<<rootData[std::string("height")];
    strm >> mapHeight;
    strm.clear();
    createTiles();
    createLayers();
}

void Mapdata::debug() const{
    std::cerr << "Tile width is " << tileWidth << std::endl;
    std::cerr << "Tile height is " << tileHeight << std::endl;
    std::cerr << "Map width is " << mapWidth << std::endl;
    std::cerr << "Map height is " << mapHeight << std::endl;
}

void Mapdata::createTiles() {
    std::list<std::map<std::string, std::string> > tileList = parser.parseNodesWithTag("tile");
    // for each map in list
    for(std::list<std::map<std::string, std::string> >::const_iterator iter=tileList.begin();iter!=tileList.end();++iter)
    {
        std::map<std::string, std::string> temp = *iter;
        tiles.insert(std::pair<std::string,std::string>(temp[std::string("id")], std::string(temp[std::string("name")])));
    }
}

void Mapdata::createLayers() 
{
    std::list<const rapidxml::xml_node<>* > layers = parser.findNodes("layer");
    // Walk through "layer nodes"
    for(std::list<const rapidxml::xml_node<>* >::const_iterator iterator = layers.begin(); iterator!= layers.end();++iterator)
    {
        std::list<Tile> newLayer;
        unsigned int i =0;
        // walk through nodes under that
        for(rapidxml::xml_node<>* node= (*iterator)->first_node();node; node=node->next_sibling())
        {
            std::string id;
            bool collision=false;
            // walk through their attributes
            for(rapidxml::xml_attribute<>* attr = node->first_attribute();attr; attr=attr->next_attribute())
            {   
                std::stringstream strm;
                strm << attr->value();
                if(strcmp(attr->name(),"id")==0)
                {
                    strm >>id;
                }
                else if(strcmp(attr->name(), "collision")==0)
                {
                    strm>>collision;
                }
                else 
                {
                    throw std::string("Bad attribute for tile \"")+attr->name()+std::string("\"");
                }
            }
            unsigned int worldWidth = Gamedata::getInstance().getXmlInt("worldWidth");
            unsigned int worldHeight = Gamedata::getInstance().getXmlInt("worldHeight");
            unsigned int offsetX = worldWidth/2 - tileWidth/2;
            unsigned int offsetY = worldHeight/2 - mapHeight*tileHeight/2;
            // by term: (middle of world) + (offset by row) + offset from middle
            unsigned int tileLocX=  ((i/mapWidth)*tileWidth/2)-((i%mapWidth)*tileWidth/2)+offsetX;
            unsigned int tileLocY= ((i/mapHeight)*tileHeight/2)+((i%mapHeight)*tileHeight/2)+offsetY;
            //unsigned int tileLocX =(worldWidth/2)-((i/mapWidth)*tileWidth)+(i*tileWidth/2)-(mapWidth*tileWidth/2);
            //unsigned int tileLocY =(worldHeight/2) -(i*tileHeight/2)+((i / mapHeight)*tileHeight*2);
            newLayer.push_back(Tile(tiles[id],Vector2f(tileLocX,tileLocY),collision));
            i++;
        }
        mapLayers.push_back(newLayer);
    }
}

// for now returns coordinate of bottom tile on bottom layer
Vector2f Mapdata::getOrigin() const {
    return ( (*(*mapLayers.begin()).begin()).getCoord() + Vector2f(0,tileHeight/2) );
}

void Mapdata::draw() const {
    for(std::list<std::list<Tile>  >::const_iterator it = mapLayers.begin(); it!=mapLayers.end(); ++it)
    {   
        for(std::list<Tile>::const_iterator layer_it = (*it).begin(); layer_it != (*it).end(); ++layer_it)
        {
            (*layer_it).draw();
        }
    }
}

void Mapdata::update(Uint32& ticks) {
    for(std::list<std::list<Tile>  >::const_iterator it = mapLayers.begin(); it!=mapLayers.end(); ++it)
    {   
        for(std::list<Tile>::const_iterator layer_it = (*it).begin(); layer_it != (*it).end(); ++layer_it)
        {
            (*layer_it).update(ticks);
        }
    }
}

void Mapdata::displayData() const {
    parser.displayData();
}

