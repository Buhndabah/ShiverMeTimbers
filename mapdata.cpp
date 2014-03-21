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
#ifdef DEBUG
    std::cerr << "Printing header information for map " << fn << std::endl;
    for(auto &t : rootData)
        std::cerr << t.first << " " << t.second << std::endl;
#endif
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
    std::list<const rapidxml::xml_node<>* > tileDefs = parser.findNodes("tile");
    for(std::list<const rapidxml::xml_node<>* >::const_iterator iterator= tileDefs.begin();iterator != tileDefs.end(); ++iterator)
    {   
        int id;
        std::string name;
        for(rapidxml::xml_attribute<>* attr=(*iterator)->first_attribute();attr;attr=attr->next_attribute())
        {
            if(strcmp(std::string(attr->name()).c_str(),"id")==0)
            {
                std::stringstream strm;
                strm << std::string(attr->value());
                strm >> id;
            }
            else if(strcmp(std::string(attr->name()).c_str(),"name")==0)
            {
                name=std::string(attr->value());
            }
            else
                throw std::string("Unknown attribute \"")+attr->name()+std::string("\" found for tile.");
        }
        tiles.insert(std::pair<int, std::string>(id, name));
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
            int id=0;
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
            unsigned int tileLocX =(worldWidth/2)-((i/mapWidth)*tileWidth)+(i*tileWidth/2)-(mapWidth*tileWidth/2);
            unsigned int tileLocY =(worldHeight/2) -(i*tileHeight/2)+((i / mapHeight)*tileHeight*2);
            
            newLayer.push_back(Tile(tiles[id],Vector2f(tileLocX,tileLocY),collision));
            i++;
        }
        mapLayers.push_back(newLayer);
    }
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
    //xmlData.displayData();
}

