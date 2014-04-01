#include <algorithm>
#include <sstream>
#include <SDL/SDL.h>
#include <iostream>
#include <math.h>
#include "viewport.h"
#include "ioManager.h"
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


    // Read in map constants
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

    // Fill vector with appropriate number of lists and then fill it
    gridElements.reserve(mapWidth*mapHeight);
    int i;
    for(i=0; i < mapHeight * mapWidth; i++)
    {   
        gridElements.push_back( std::list<GridElement* >());
    }

    createTiles();
    createLayers();
}

MapManager::~MapManager() {
    std::list<GridElement*> list;
    while (!gridElements.empty())
    {
        list=gridElements.front();
        while(!list.empty())
        {
            delete list.front();
            list.erase(list.begin());
        }
        gridElements.erase(gridElements.begin());
    }
}

void MapManager::debug() const{
    std::cerr << "Tile width is " << tileWidth << std::endl;
    std::cerr << "Tile height is " << tileHeight << std::endl;
    std::cerr << "Map width is " << mapWidth << std::endl;
    std::cerr << "Map height is " << mapHeight << std::endl;
}

void MapManager::addGridElement(GridElement* gridE) {
    gridElements[getIndexAt(gridE->getGridPosition()+gridE->getSprite().getSize())].push_back(gridE);
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

// Walks through xml data, creating and filling layers with tiles
void MapManager::createLayers() 
{
    std::list<const rapidxml::xml_node<>* > layers = parser.findNodes("layer");
    std::vector<Tile> newLayer;
    std::string id;
    std::stringstream strm;
    unsigned int tileIndex=0;
    std::string collision;
    unsigned int layerIndex=0;
    std::vector<Tile*> topOfStack(mapWidth*mapHeight);
    unsigned int worldWidth = Gamedata::getInstance().getXmlInt("worldWidth");
    unsigned int worldHeight = Gamedata::getInstance().getXmlInt("worldHeight");


    // Walk through layer tags (height levels)
    for(std::list<const rapidxml::xml_node<>* >::const_iterator iterator = layers.begin(); iterator!= layers.end();++iterator)
    {
        tileIndex =0;

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
            int offsetX = worldWidth/2 - tileWidth/2;
            int offsetY = worldHeight/2 - mapHeight*tileHeight/2+100;
            int offsetZ = layerIndex*tileRise;
            int tileLocX=  ((tileIndex/mapWidth)*tileWidth/2)-((tileIndex%mapWidth)*tileWidth/2)+offsetX;
            int tileLocY= ((tileIndex/mapHeight)*tileHeight/2)+((tileIndex%mapHeight)*tileHeight/2)+offsetY-offsetZ;

            newLayer.push_back(Tile(id,tiles[id],Vector2f(tileLocX,tileLocY),collision.compare("true") ? true : false));
            tileIndex++;
        }
        mapLayers.push_back(newLayer);
        newLayer.clear();

        // Update highest tile at each index
        std::vector<Tile>::iterator it=mapLayers.back().begin();
        for(int j=0; it+j!=mapLayers.back().end(); ++j)
        {
            if(layerIndex==0)
            {
                topOfStack[j] = &(*(it+j));
            }
            else if((*(it+j)).getId().compare("0")!=0)
            {
                topOfStack[j] = &(*(it+j));
            }
        }
        ++layerIndex;
    }

    // Create particle systems if a weather tag is present
    for(std::vector<Tile*>::const_iterator weatherIt=topOfStack.begin(); weatherIt!=topOfStack.end();++weatherIt)
    {
        if(weather.compare("snow")==0)
        {
            (*weatherIt)->setParticleSystem(new ParticleSystem((*weatherIt)->getCoord(),Vector2f(tileWidth,tileHeight),(*weatherIt)->getCoord()[1]+2*tileHeight));
            
        }
    }
}

// Returns coordinate of beginning of tile list on bottom layer
Vector2f MapManager::getOrigin() const {
    return ( (*(*mapLayers.begin()).begin()).getCoord()); //- Vector2f(0,tileHeight/2) );
}

/* Returns reference to a tile, given grid coordinates
   grid coordinates come in as sqrt(tileWidth^2 + tileHeight^2), 0,0 top, 355,355 bottom */
const Tile& MapManager::findTileAt(const Vector2f& coord) const {

    std::string errMess;
    std::stringstream strm;
    unsigned int indexX = coord[0]/sqrt(pow(tileWidth/2,2) + pow(tileHeight/2,2)-1);
    unsigned int indexY = coord[1]/sqrt(pow(tileWidth/2,2) + pow(tileHeight/2,2)-1);
    unsigned int i =0;

    for(std::vector<Tile>::const_iterator it = (*mapLayers.begin()).begin(); it != (*mapLayers.end()).end(); ++it)
    {
        if(i!= (indexX + (indexY*mapHeight)))
        {
            ++i;
        }
        else
        {
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
    if((hypoPos[0] > mapWidth * getGridTileWidth()))
       {
      hypoPos[0] = mapWidth * getGridTileWidth();
      dist = hypoPos[0] - g.getGridPosition()[0];

      if(g.getMoveDir()[3]) //if moving down on the screen
        hypoPos[1] = g.getGridPosition()[1] + dist;
      if(g.getMoveDir()[6]) //if moving left on the screen
        hypoPos[1] = g.getGridPosition()[1] - dist;

      dist = sqrt( pow(hypoPos[0] - g.getGridPosition()[0],2) + pow(hypoPos[1] - g.getGridPosition()[1],2));
      fticks = 1000 * dist / static_cast<float>(g.getMoveSpeed());
      atEdge = true;
    }

    // JOHN'S SUPER HACKY MOVEMENT CODE XXX TODO I'M SO SORRY
    else if((getIndexAt(hypoPos+g.getSprite().getSize())+1)%(mapWidth) == 0)
    {
         std::cerr<< getIndexAt(hypoPos+g.getSprite().getSize()) << std::endl;
         hypoPos =g.getGridPosition();
         fticks = 1000 * dist / static_cast<float>(g.getMoveSpeed());
         atEdge = true;
    }
    //Check min X border
    else if(hypoPos[0] < 0){
      hypoPos[0] = 0;
      dist = 0.-g.getGridPosition()[0];

      if(g.getMoveDir()[0]) //if moving up on the screen
        hypoPos[1] = g.getGridPosition()[1] + dist;
      if(g.getMoveDir()[7]) //if moving right on the screen
        hypoPos[1] = g.getGridPosition()[1] - dist;

      dist = sqrt( pow(hypoPos[0] - g.getGridPosition()[0],2) + pow(hypoPos[1] - g.getGridPosition()[1],2));
      fticks = 1000 * dist / static_cast<float>(g.getMoveSpeed());
      atEdge = true;
    }
 
    //Check max Y border
    else if(hypoPos[1] > mapHeight * getGridTileHeight() ){
      hypoPos[1] = mapHeight * getGridTileHeight();
      dist = hypoPos[1] - g.getGridPosition()[1];

      if(g.getMoveDir()[3]) //if moving down on the screen
        hypoPos[0] = g.getGridPosition()[0] + dist;
      if(g.getMoveDir()[7]) //if moving right on the screen
        hypoPos[0] = g.getGridPosition()[0] - dist;

      dist = sqrt( pow(hypoPos[0] - g.getGridPosition()[0],2) + pow(hypoPos[1] - g.getGridPosition()[1],2));
      fticks = 1000 * dist / static_cast<float>(g.getMoveSpeed());
      atEdge = true;
    }

    // JOHN'S SUPER HACKY MOVEMENT CODE XXX TODO I'M SO SORRY
    else if(getIndexAt(hypoPos+g.getSprite().getSize()) > mapWidth*mapHeight)
     {
         hypoPos =g.getGridPosition();
         fticks = 1000 * dist / static_cast<float>(g.getMoveSpeed());
         atEdge = true;
     }

    //Check min Y border
    else if(hypoPos[1] < 0){
      hypoPos[1] = 0;
      dist = 0.-g.getGridPosition()[1];

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


void MapManager::draw() const {
    int max=0;
    int index;
        // For each tile in each layer, draw
        for(int i=0; i<mapWidth+mapHeight-1; ++i)
         {
            for(std::list<std::vector<Tile>  >::const_iterator it = mapLayers.begin(); it!=mapLayers.end(); ++it)
            {  
             if(i<mapWidth) // top half of diamond
             {
                max = i+1;
             }
             else if(i==mapWidth) // middle row
             {
                 max= mapWidth-1;
             }
             else if(i>mapWidth) // bottom half of diamond
             {
                max = mapWidth - i%mapWidth-1;
             }
             for(int j=0;j<max; ++j)
             {
                 if(i < mapWidth)   // increasing row lengths
                 {
                    index = i + (j*(mapWidth-1));
                 }
                 else               // decreasing
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
    
    // Draw each grid element within the list corresponding to <index>
    if(gridElements[index].empty()) return;
    for(std::list<GridElement* >::const_iterator it = gridElements[index].begin(); it!=gridElements[index].end(); ++it)
    {

        /* If enabled will draw a box around the sprite boundaries */
        #ifdef HITBOX
        SDL_Rect rect;
        Uint32 color;
                 
        rect.x = (*it)->getPosition()[0] - Viewport::getInstance().X();
        rect.y = (*it)->getPosition()[1]- Viewport::getInstance().Y();
        rect.w = (*it)->getSprite().getW();
        rect.h= (*it)->getSprite().getH();
        color = SDL_MapRGB(IOManager::getInstance().getScreen()->format,0,0,0);
        SDL_FillRect(IOManager::getInstance().getScreen(), &rect, color);
        #endif
        
         
        (*it)->draw();

    }
}

void MapManager::update(Uint32& ticks) {

    // update each tile in each layer
    for(std::list<std::vector<Tile>  >::const_iterator it = mapLayers.begin(); it!=mapLayers.end(); ++it)
    {   
        for(std::vector<Tile>::const_iterator layer_it = (*it).begin(); layer_it != (*it).end(); ++layer_it)
        {
            (*layer_it).update(ticks);
        }
    }


    // This is a temp list used to store GridElements as we sort them by index
    std::vector<std::list<GridElement *> > tempVec;
    tempVec.reserve(mapWidth*mapHeight);
    for(int i=0; i < mapWidth*mapHeight; i++)
    {   
        tempVec.push_back(std::list<GridElement*>());
    }

    // Walk through Grid elements, remove them from the list, update, and then find their new position
    GridElement* element;
    for(std::vector<std::list<GridElement*> >::iterator it=gridElements.begin(); it!=gridElements.end(); ++it)
       {
           if(!(*it).empty())
           {
               while(!(*it).empty())
               {
                   element = (*it).front();
                   (*it).pop_front();
                   element->update(ticks);
   
                   tempVec[getIndexAt(element->getGridPosition()+element->getSprite().getSize())].push_back(element);
               }
           }
       }
       // copy temp list into permanent list
       gridElements = std::vector<std::list<GridElement*> >(tempVec);
}

/* Give a grid coordinate, will return the index of the tile underneath that coordinate, if any. */
// XXX TODO Should probably retrn some unique value if out of bounds
int MapManager::getIndexAt(const Vector2f& coord) const {
    int indexX = floor(coord[0]/sqrt(pow(tileWidth/2,2) + pow(tileHeight/2,2)))-1;
    int indexY = floor(coord[1]/sqrt(pow(tileWidth/2,2) + pow(tileHeight/2,2)))-1;
    return indexX+(indexY*mapHeight);
}

// Spit out what the parser is storing
void MapManager::displayData() const {
    parser.displayData();
}

