#include <algorithm>
#include <sstream>
#include <SDL/SDL.h>
#include <iostream>
#include <math.h>
#include <stdexcept>
#include <algorithm>
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
    Listener(),
    parser(XMLParser::getInstance()),
    tiles(),
    updateTiles(),
    mapLayers(),
    gridElements(),
    tileWidth(),
    tileHeight(),
    tileRise(),
    mapWidth(),
    mapHeight(),
    weather()
{
    parser.parse(fn);

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

// Returns coordinate of beginning of tile list on bottom layer
Vector2f MapManager::getOrigin() const {
    return ( (*(*mapLayers.begin()).begin()).getCoord()) + Vector2f(tileWidth/2,0);
}

// Translates a position on the grid to a position in the world
Vector2f MapManager::gridToWorld(Vector2f gridPos) const {
    Vector2f ret = getOrigin();
    ret[0] -= gridPos[0] * cos(26.565*M_PI/180);
    ret[0] += gridPos[1] * cos(26.565*M_PI/180);
    ret[1] += (gridPos[0] * sin(26.565*M_PI/180)) + (gridPos[1] * sin(26.565*3.141592653589/180));
    return ret;
}

//Translates a position in the world to a position on the grid
Vector2f MapManager::worldToGrid(Vector2f worldPos) const {
    Vector2f diff = worldPos - getOrigin();
    Vector2f ret(0,0);

    ret[0] -= diff[0] / cos(26.565 * M_PI/180) * 0.5;
    ret[1] += diff[0] / cos(26.565 * M_PI/180) * 0.5;

    ret[0] += diff[1] / cos(26.565 * M_PI/180);
    ret[1] += diff[1] / cos(26.565 * M_PI/180);

    return ret;
}

// XXX THE EXCEPTION IN THIS FUNCTION ISN'T CATCHING WHEN YOU TRY TO ADD TO AN INVALID INDEX
void MapManager::addGridElement(GridElement* gridE) {

    //Find the max index between the bottom corner of the movebox and the bottom right corner of the gridSprite
    float diffX = gridToWorld(gridE->getMoveboxVertices()[2])[0] - gridToWorld(gridE->getMoveboxVertices()[3])[0];
    int index = std::max(getIndexAt(worldToGrid(gridToWorld(gridE->getMoveboxVertices()[3]))),
					getIndexAt(worldToGrid(gridToWorld(gridE->getMoveboxVertices()[3]) + Vector2f(diffX,0))));
    index = std::max(index,getIndexAt(worldToGrid(gridToWorld(gridE->getMoveboxVertices()[3]) + Vector2f(diffX/2.,0))));
    index = std::max(index,getIndexAt(worldToGrid(gridToWorld(gridE->getMoveboxVertices()[3]) + Vector2f(-diffX/2.,0))));


    try{ gridElements[index].push_back(gridE); }
    catch(const std::out_of_range& e) {
        std::cerr << "Tried to add GridElement with name " << gridE->getSprite().getName() << " to map at invalid grid position " << gridE->gridX() << ", " << gridE->gridY() << std::endl;
    }
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
    std::stringstream nameStrm;
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
            std::string strName;
            nameStrm << tileLocX;
            nameStrm >> strName;
            nameStrm.clear();
            nameStrm << tileLocY;
            strName = strName + nameStrm.str(); 
            newLayer.push_back(Tile(strName, id,tiles[id],Vector2f(tileLocX,tileLocY),collision.compare("true") ? true : false));
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
    for(std::vector<Tile*>::iterator weatherIt=topOfStack.begin(); weatherIt!=topOfStack.end();++weatherIt)
    {
        if(weather.compare("snow")==0)
        {
            (*weatherIt)->addParticleSystem(2*tileHeight,weather);
            updateTiles.push_back((*weatherIt)); 
        }
    }
}

/* Returns reference to a tile, given grid coordinates
   grid coordinates come in as sqrt(tileWidth^2 + tileHeight^2), 0,0 top, 355,355 bottom */
const Tile& MapManager::findTileAt(const Vector2f& coord) const {

    std::string errMess;
    std::stringstream strm;
  //  unsigned int indexX = coord[0]/sqrt(pow(tileWidth/2,2) + pow(tileHeight/2,2)-1);
  //  unsigned int indexY = coord[1]/sqrt(pow(tileWidth/2,2) + pow(tileHeight/2,2)-1);
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
            return (*it);
        }
    }

    strm << "Request for tile@" << coord[0] << ", " << coord[1] << "failed.\n"
         << "Translated index is " << (indexX +(indexY*mapHeight)) << std::endl;
    strm >> errMess;
    throw errMess;
}


/* Checks if a position after the passed in hypothetical increase is within the grid
   If valid, return hypothetical position
   If invalid, return the valid movement
   The time measure, fticks, is passed by reference to allow it to be adjusted if the movement is cut short
   The boolean, atEdge, is passed by reference to let the caller know if the movement was cut short
*/
Vector2f MapManager::validateMovement(GridElement& g, Vector2f hypoIncr, float& fticks, bool& atEdge) const{

    Vector2f validPos = g.getGridPosition() + hypoIncr;

    std::vector<Vector2f> movebox = g.getMoveboxVertices();
    //check each corner of the movebox
    for(int i=0; i<4; ++i){
    	if(!findTileAt(movebox[i] + hypoIncr).isCollidable()){
	    validPos = g.getGridPosition();
	    atEdge = true;
	}
    }

    Tile tile;
    //check midpoints between corners
    float diffX = movebox[1][0] - movebox[0][0];  
    float diffY = movebox[2][1] - movebox[0][1];  
    if(!(tile = Tile(findTileAt(movebox[0] + Vector2f(diffX/2.,0) + hypoIncr))).isCollidable()
		||!(tile = Tile(findTileAt(movebox[0] + Vector2f(0,diffY/2.) + hypoIncr))).isCollidable()
		||!(tile = Tile(findTileAt(movebox[0] + Vector2f(diffX,diffY/2.) + hypoIncr))).isCollidable()
		||!(tile = Tile(findTileAt(movebox[0] + Vector2f(diffX/2.,diffY) + hypoIncr))).isCollidable()){
        validPos = g.getGridPosition();
        atEdge = true;
    }
    else
    {
        GameEvents::Event e;
        e.type = GameEvents::COLLIDE_EVENT;
        e.actor = g.getName();
        e.subject = tile.getName();
        e.location = g.getPosition();
        GameEvents::EventQueue::getInstance().push(e);
    }

    float dist = sqrt( pow(validPos[0] - g.getGridPosition()[0],2) + pow(validPos[1] - g.getGridPosition()[1],2));
    fticks = 1000 * dist / static_cast<float>(g.getMoveSpeed());


    return validPos;
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
                if(it==(mapLayers.begin()))
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
	#define HITBOX
        #ifdef HITBOX
        SDL_Rect rect;
        Uint32 color;
                 
        rect.x = (*it)->getPosition()[0] - Viewport::getInstance().X();
        rect.y = (*it)->getPosition()[1]- Viewport::getInstance().Y();
        rect.w = (*it)->getSprite().getW();
        rect.h= (*it)->getSprite().getH();
        color = SDL_MapRGB(IOManager::getInstance().getScreen()->format,0,0,0);
        SDL_FillRect(IOManager::getInstance().getScreen(), &rect, color);

	SDL_Rect gp;
        gp.x = (*it)->getPosition()[0] - Viewport::getInstance().X();
        gp.y = (*it)->getPosition()[1]- Viewport::getInstance().Y();
        gp.w = 10;
        gp.h= 10;
        Uint32 color2 = SDL_MapRGB(IOManager::getInstance().getScreen()->format,255,0,0);
        SDL_FillRect(IOManager::getInstance().getScreen(), &gp, color2);
 
	SDL_Rect c;
        c.x = (*it)->getPosition()[0] + (*it)->getSprite().getW()/2 - Viewport::getInstance().X();
        c.y = (*it)->getPosition()[1]- Viewport::getInstance().Y();
        c.w = 10;
        c.h= 10;
        color2 = SDL_MapRGB(IOManager::getInstance().getScreen()->format,255,255,0);
        SDL_FillRect(IOManager::getInstance().getScreen(), &c, color2);
 
	SDL_Rect e;
        e.x = (*it)->getPosition()[0] + (*it)->getSprite().getW() - Viewport::getInstance().X();
        e.y = (*it)->getPosition()[1]- Viewport::getInstance().Y();
        e.w = 10;
        e.h= 10;
        color2 = SDL_MapRGB(IOManager::getInstance().getScreen()->format,0,0,255);
        SDL_FillRect(IOManager::getInstance().getScreen(), &e, color2);

        (*it)->draw();

	for(int i=0; i<4; ++i){
	    SDL_Rect mb;
            mb.x = gridToWorld((*it)->getMoveboxVertex(i))[0] - Viewport::getInstance().X();
            mb.y = gridToWorld((*it)->getMoveboxVertex(i))[1] - Viewport::getInstance().Y();
	    mb.w = 10;
	    mb.h= 10;
	    Uint32 color2 = SDL_MapRGB(IOManager::getInstance().getScreen()->format,0,255,0);
            SDL_FillRect(IOManager::getInstance().getScreen(), &mb, color2);
	} 
         
        #endif

    }
}

void MapManager::update(Uint32& ticks) {

    // update each tile in each layer
    /*for(std::list<std::vector<Tile>  >::const_iterator it = mapLayers.begin(); it!=mapLayers.end(); ++it)
    {   
        for(std::vector<Tile>::const_iterator layer_it = (*it).begin(); layer_it != (*it).end(); ++layer_it)
        {
            (*layer_it).update(ticks);
        }
    }*/
    for(std::list<Tile*>::const_iterator it=updateTiles.begin(); it!= updateTiles.end(); ++it)
    {
        (*it)->update(ticks);
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
  
                    
                   //try {tempVec[getIndexAt(element->getGridPosition()+element->getSprite().getSize())].push_back(element); }

		   //Find the max index between the bottom corner of the movebox and the bottom right corner of the gridSprite
    		   float diffX = gridToWorld(element->getMoveboxVertices()[2])[0] - gridToWorld(element->getMoveboxVertices()[3])[0];
		   int index = std::max(getIndexAt(worldToGrid(gridToWorld(element->getMoveboxVertices()[3]))),
					getIndexAt(worldToGrid(gridToWorld(element->getMoveboxVertices()[3]) + Vector2f(diffX,0))));

		   index = std::max(index,getIndexAt(worldToGrid(gridToWorld(element->getMoveboxVertices()[3]) + Vector2f(diffX/2.,0))));
		   index = std::max(index,getIndexAt(worldToGrid(gridToWorld(element->getMoveboxVertices()[3]) + Vector2f(-diffX/2.,0))));

                   try {tempVec[index].push_back(element); }
                   catch(const std::out_of_range& e) {
                       std::cerr << "Tried to access GridElement at index " << index << std::endl;
                   }
               }
           }
       }
       // copy temp list into permanent list
       gridElements = std::vector<std::list<GridElement*> >(tempVec);
}

/* Give a grid coordinate, will return the index of the tile underneath that coordinate, if any. */
// returns a negative on out of bounds
// -1 = minX
// -2 = minY
// -3 = maxX
// -4 = maxY
int MapManager::getIndexAt(const Vector2f& coord) const {
    int indexX = floor(coord[0]/sqrt(pow(tileWidth/2,2) + pow(tileHeight/2,2)));
    int indexY = floor(coord[1]/sqrt(pow(tileWidth/2,2) + pow(tileHeight/2,2)));

    // Check for out of bounds index
    if(indexX < 0) { return -1; } 
    if(indexY < 0) { return -2; }
    if(indexX >= mapWidth)  { return -3; }
    if(indexY >= mapHeight) { return -4; }

    return indexX+(indexY*mapHeight);
}

// Spit out what the parser is storing
void MapManager::displayData() const {
    parser.setCurDocument("xmlSpec/testMap.xml");
    parser.displayData();
}

void MapManager::registerListeners() {}
