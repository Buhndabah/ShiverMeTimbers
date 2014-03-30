#include <algorithm>
#include <sstream>
#include <iostream>
#include <math.h>
#include "vector2f.h"
#include "gamedata.h"
#include "mapManager.h"

MapManager& MapManager::getInstance() {
    static MapManager instance;
    return instance;
}

MapManager::MapManager(const std::string& fn) :
    parser(fn),
    tiles(),
    mapLayers(),
    gridElements(),
    tileWidth(),
    tileHeight(),
    tileRise(),
    mapWidth(),
    mapHeight(),
    weather()
{

    std::stringstream strm; 
    std::map<std::string, std::string> rootData = parser.parseNode(parser.findNodes("map").front());

    strm<<rootData[std::string("tilewidth")];
    strm >> tileWidth;
    strm.clear();
    strm<<rootData[std::string("tileheight")];
    strm >> tileHeight;
    strm.clear();
    strm<<rootData[std::string("tileRise")];
    strm >> tileRise;
    strm.clear();
    strm << rootData[std::string("width")];
    strm >> mapWidth;
    strm.clear();
    strm<<rootData[std::string("height")];
    strm >> mapHeight;
    strm.clear();
    strm<<rootData[std::string("weather")];
    strm >> weather;
    strm.clear();

    for(int i=0; i < tileHeight*tileWidth; i++)
    {   
        std::list<GridElement*> newList;
        gridElements.push_back(newList);
    }

    createTiles();
    createLayers();
}

void MapManager::debug() const{
    std::cerr << "Tile width is " << tileWidth << std::endl;
    std::cerr << "Tile height is " << tileHeight << std::endl;
    std::cerr << "Map width is " << mapWidth << std::endl;
    std::cerr << "Map height is " << mapHeight << std::endl;
}

// Parse tile definitions (not the actual objects though)
void MapManager::createTiles() {
    std::list<std::map<std::string, std::string> > tileList = parser.parseNodesWithTag("tile");

    // for each map in list
    for(std::list<std::map<std::string, std::string> >::const_iterator iter=tileList.begin();iter!=tileList.end();++iter)
    {
        std::map<std::string, std::string> temp = *iter;
        tiles.insert(std::pair<std::string,std::string>(temp[std::string("id")], std::string(temp[std::string("name")])));
    }
}

// The meat of it, create the tile objects
void MapManager::createLayers() 
{
    std::list<const rapidxml::xml_node<>* > layers = parser.findNodes("layer");
    std::vector<Tile> newLayer;
    std::string id;
    std::stringstream strm;
    unsigned int i=0;
    std::string collision;
    unsigned int layerIndex=0;

    // Walk through layer tags (height levels)
    for(std::list<const rapidxml::xml_node<>* >::const_iterator iterator = layers.begin(); iterator!= layers.end();++iterator)
    {
        i =0;

        // walk through tile ids in layer
        for(rapidxml::xml_node<>* node= (*iterator)->first_node();node; node=node->next_sibling())
        {  
            collision="false"; 
            // walk through their attributes
            for(rapidxml::xml_attribute<>* attr = node->first_attribute();attr; attr=attr->next_attribute())
            {   
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
                strm.clear();
            }

            // Calculate draw coordinates for new tile
            unsigned int worldWidth = Gamedata::getInstance().getXmlInt("worldWidth");
            unsigned int worldHeight = Gamedata::getInstance().getXmlInt("worldHeight");
            unsigned int offsetX = worldWidth/2 - tileWidth/2;
            unsigned int offsetY = worldHeight/2 - mapHeight*tileHeight/2;
            unsigned int offsetZ = layerIndex*tileRise;
            unsigned int tileLocX=  ((i/mapWidth)*tileWidth/2)-((i%mapWidth)*tileWidth/2)+offsetX;
            unsigned int tileLocY= ((i/mapHeight)*tileHeight/2)+((i%mapHeight)*tileHeight/2)+offsetY-offsetZ;

            newLayer.push_back(Tile(tiles[id],Vector2f(tileLocX,tileLocY),collision.compare("true") ? true : false));
            if(weather.compare("snow")==0)
            {
                newLayer.back().setParticleSystem(new ParticleSystem(Vector2f(tileLocX,tileLocY),Vector2f(tileWidth,tileHeight),worldHeight-tileLocY));
            }
            i++;
        }
        mapLayers.push_back(newLayer);
        ++layerIndex;
        newLayer.clear();
    }
}

// Returns coordinate of beginning of tile list on bottom layer
Vector2f MapManager::getOrigin() const {
    std::cerr<< ((*(*mapLayers.begin()).begin()).getCoord()) << std::endl;
    return ( (*(*mapLayers.begin()).begin()).getCoord()); //- Vector2f(0,tileHeight/2) );
}

/* Returns reference to a tile, given grid coordinates
   grid coordinates come in as sqrt(tileWidth^2 + tileHeight^2), 0,0 top, 355,355 bottom */
const Tile& MapManager::findTileAt(const Vector2f& coord) const {

    std::string errMess;
    std::stringstream strm;
    unsigned int indexX = coord[0]/sqrt(pow(tileWidth/2,2) + pow(tileHeight/2,2));
    unsigned int indexY = coord[1]/sqrt(pow(tileWidth/2,2) + pow(tileHeight/2,2));
    unsigned int i =0;

    for(std::vector<Tile>::const_iterator it = (*mapLayers.begin()).begin(); it != (*mapLayers.end()).end(); ++it)
    {
        if(i!= (indexX + (indexY*mapHeight)))
        {
            ++i;
        }
        else
        {
            std::cerr << "tile found was # " << i << std::endl;
            return (*it);
        }
    }

    strm << "Request for tile@" << coord[0] << ", " << coord[1] << "failed.\n"
         << "Translated index is " << (indexX +(indexY*mapHeight)) << std::endl;
    strm >> errMess;
    throw errMess;
}

/* Checks if a hypothetical position is within the grid
   If valid, return hypothetical position
   If invalid, return position at the edge of the grid
   The time measure, fticks, is passed by reference to allow it to be adjusted if the movement is cut short
   The boolean, atEdge, is passed by reference to let the caller know if the movement was cut short
*/
Vector2f MapManager::validateMovement(GridElement& g, Vector2f hypoPos, float& fticks, bool& atEdge) const{

    float dist = 0.;
    //Check max X border
    if(hypoPos[0] > mapWidth * getTileWidth()){
      hypoPos[0] = mapWidth * getTileWidth();
      dist = hypoPos[0] - g.getGridPosition()[0];

      if(g.getMoveDir()[3]) //if moving down on the screen
        hypoPos[1] = g.getGridPosition()[1] + dist;
      if(g.getMoveDir()[6]) //if moving left on the screen
        hypoPos[1] = g.getGridPosition()[1] - dist;

      dist = sqrt( pow(hypoPos[0] - g.getGridPosition()[0],2) + pow(hypoPos[1] - g.getGridPosition()[1],2));
      fticks = 1000 * dist / static_cast<float>(g.getMoveSpeed());
      atEdge = true;
    }
    //Check min X border
    else if(hypoPos[0] < 0){
      hypoPos[0] = 0;
      dist = -g.getGridPosition()[0];

      if(g.getMoveDir()[0]) //if moving up on the screen
        hypoPos[1] = g.getGridPosition()[1] + dist;
      if(g.getMoveDir()[7]) //if moving right on the screen
        hypoPos[1] = g.getGridPosition()[1] - dist;

      dist = sqrt( pow(hypoPos[0] - g.getGridPosition()[0],2) + pow(hypoPos[1] - g.getGridPosition()[1],2));
      fticks = 1000 * dist / static_cast<float>(g.getMoveSpeed());
      atEdge = true;
    }
 
    //Check max Y border
    else if(hypoPos[1] > mapHeight * getTileHeight()){
      hypoPos[1] = mapHeight * getTileHeight();
      dist = hypoPos[1] - g.getGridPosition()[1];

      if(g.getMoveDir()[3]) //if moving down on the screen
        hypoPos[0] = g.getGridPosition()[0] + dist;
      if(g.getMoveDir()[7]) //if moving right on the screen
        hypoPos[0] = g.getGridPosition()[0] - dist;

      dist = sqrt( pow(hypoPos[0] - g.getGridPosition()[0],2) + pow(hypoPos[1] - g.getGridPosition()[1],2));
      fticks = 1000 * dist / static_cast<float>(g.getMoveSpeed());
      atEdge = true;
    }
 
    //Check min Y border
    else if(hypoPos[1] < 0){
      hypoPos[1] = 0;
      dist = -g.getGridPosition()[1];

      if(g.getMoveDir()[0]) //if moving down on the screen
        hypoPos[0] = g.getGridPosition()[0] + dist;
      if(g.getMoveDir()[6]) //if moving right on the screen
        hypoPos[0] = g.getGridPosition()[0] - dist;

      dist = sqrt( pow(hypoPos[0] - g.getGridPosition()[0],2) + pow(hypoPos[1] - g.getGridPosition()[1],2));
      fticks = 1000 * dist / static_cast<float>(g.getMoveSpeed());
      atEdge = true;
    }

    return hypoPos;
}

// For each tile in each layer, draw
void MapManager::draw() const {
    int max=0;
    int index;
    for(std::list<std::vector<Tile>  >::const_iterator it = mapLayers.begin(); it!=mapLayers.end(); ++it)
    {  
        for(int i=0; i<mapWidth+mapHeight-1; ++i)
         {
             if(i<mapWidth)
             {
                max = i+1;
             }
             else if(i==mapWidth)
             {
                 max= mapWidth-1;
             }
             else if(i>mapWidth)
             {
                max = mapWidth - i%mapWidth-1;
             }
             for(int j=0;j<max; ++j)
             {
                 if(i < mapWidth)
                 {
                    index = i + (j*(mapWidth-1));
                 }
                 else
                 {
                    index = i+ ((i+1)%mapWidth)*(mapWidth-1) +(j*(mapWidth-1));
                 }

                (*it)[index].draw();
                drawGridElements(index);
             }
         }
    }
}

void MapManager::drawGridElements(int index) const {
    if(gridElements.empty()) return;
    for(std::list<GridElement* >::const_iterator it = gridElements[index].begin(); it!=gridElements[index].end(); ++it)
    {
        (*it)->draw();
    }
}

// For each tile in each layer, update
void MapManager::update(Uint32& ticks) {
    for(std::list<std::vector<Tile>  >::const_iterator it = mapLayers.begin(); it!=mapLayers.end(); ++it)
    {   
        for(std::vector<Tile>::const_iterator layer_it = (*it).begin(); layer_it != (*it).end(); ++layer_it)
        {
            (*layer_it).update(ticks);
        }
    }


    for(std::vector<std::list<GridElement*> >::const_iterator it=gridElements.begin(); it!=gridElements.end(); ++it)
       {
           if(!(*it).empty())
           {
               for(std::list<GridElement*>::const_iterator geIt =(*it).begin(); geIt!=(*it).end(); ++geIt)
               {
                   (*geIt)->update(ticks);
               }
           }
       }
    
}

// Spit out what the parser is storing
void MapManager::displayData() const {
    parser.displayData();
}

