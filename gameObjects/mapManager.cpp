#include <algorithm>
#include <sstream>
#include <SDL/SDL.h>
#include <iterator>
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
    numGridElements(0),
    player(NULL),
    dummyTile(NULL),
    tiles(),
    reserve(),
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
    // load map into parser
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

    // Empty tile, used in error handling
    dummyTile = new Tile();

    createTiles();
    createLayers();
    registerListeners();
}

void MapManager::reinit() {

    // dump all grid elements that have been created
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

    // put <MAPSIZE> lists back into grid element
    gridElements.reserve(mapWidth*mapHeight);
    int i;
    for(i=0; i < mapHeight * mapWidth; i++)
    {   
        gridElements.push_back( std::list<GridElement* >());
    }

    // dump our list of temp gridElements
    while(!reserve.empty())
    {
        GridElement* temp = (*reserve.begin()).first;
        reserve.erase(reserve.begin());
        delete temp;
    }

    // the event queue got reset - reregister ourself
    registerListeners();
}

MapManager::~MapManager() {

    // dump all of our allocated lists
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
    while(!reserve.empty())
    {
        GridElement* temp = (*reserve.begin()).first;
        reserve.erase(reserve.begin());
        delete temp;
    }

    delete dummyTile;
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

    // increment gridElement counter
    numGridElements++;

    //Find the max index between the bottom corner of the movebox and the bottom right corner of the gridSprite
    
    // Calculate how wide the gridElement is
    float diffX = gridToWorld(gridE->getMoveboxVertices()[2])[0] - gridToWorld(gridE->getMoveboxVertices()[3])[0];

    // XXX why are we converting grid to world then world to grid here?
    
    // find which is further a full length on either side? (???)
    int index = getIndexAt(gridE->getMoveboxVertices()[3] + Vector2f(diffX,0));

    // THIS PART BREAKS
    try{ gridElements.at(index).push_back(gridE); }
    catch(const std::out_of_range& e) {
        std::cerr << "Tried to add GridElement with name " << gridE->getSprite().getName() << " to map at invalid grid position " << gridE->gridX() << ", " << gridE->gridY() << std::endl;
    }
}


// Remove grid element from list
void MapManager::removeGridElement(const std::string& name) {
    
    // search the list for <name>
    for(std::vector<std::list<GridElement*> >::iterator it = gridElements.begin(); it!= gridElements.end(); ++it)
    {
        std::list<GridElement*>::iterator it2=(*it).begin();
        while(it2 != (*it).end())
        {
            if((*it2)->getName().compare(name)==0)
            {
                delete (*it2);
                (*it).erase(it2++);
            }
            else 
            {
                it2++;
            }
        }
    }
}

// Parse tile definitions
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
    int indexX = coord[0]/sqrt(pow(tileWidth/2,2) + pow(tileHeight/2,2));
    int indexY = coord[1]/sqrt(pow(tileWidth/2,2) + pow(tileHeight/2,2));


    // check for in bounds index
    // return uninit tile
    if( (indexX < 0) || (indexY < 0) || (indexX >= mapWidth) || (indexY >= mapHeight) ) {
        return *dummyTile;
    }

    // walk to the calculated index
    unsigned int i =0;
    std::vector<Tile>::const_iterator it = (*mapLayers.begin()).begin();
    while(i != (indexX + (indexY*mapHeight))) {
        ++i;
        ++it;
    }
    return (*it);

    /*strm << "Request for tile@" << coord[0] << ", " << coord[1] << "failed.\n"
         << "Translated index is " << (indexX +(indexY*mapHeight)) << std::endl;
    strm >> errMess;
    throw errMess;*/
}

/*Helper function*/
// XXX TODO this is horribly inefficient right now, it checks every gridElement against every gridElement
void MapManager::collideGridEles(int tileIndx, GridElement* g) const{

    Vector2f hypoIncr = g->getMoveDelta();

    // check collision against every grid element
    std::list<GridElement *>::const_iterator iter;
    for(iter = (gridElements[tileIndx]).begin(); iter != (gridElements[tileIndx]).end(); ++iter){

        // if we try to collide ourselves?
	if(g == *iter) {
            continue;
        }

	std::vector<Vector2f> movebox = g->getMoveboxVertices();
	std::vector<Vector2f> testmovebox = (*iter)->getMoveboxVertices();
	for(int j=0; j<4; ++j){
	    bool minX = (movebox[j] + hypoIncr)[0] >= (testmovebox[0])[0];
	    bool minY = (movebox[j] + hypoIncr)[1] >= (testmovebox[0])[1];
	    bool maxX = (movebox[j] + hypoIncr)[0] <= (testmovebox[1])[0];
	    bool maxY = (movebox[j] + hypoIncr)[1] <= (testmovebox[2])[1];

	    bool inBounds = (minX && maxX && minY && maxY);

	    if(inBounds && g->getSolid() && (*iter)->getSolid()){
                int myStrat = g->getStratType();
                int theirStrat = (*iter)->getStratType();

                // bullets ignore each other
                if( (g->getStratType() == (*iter)->getStratType()) && g->getStratType()==BULLET_STRAT) 
                {
                    // nothing
                }
                // if bullet strat, check that the source isn't us
                else if( (myStrat == BULLET_STRAT && dynamic_cast<BulletStrategy*>(g->getStrat())->getSource().compare((*iter)->getName())==0) 
                          ||(theirStrat == BULLET_STRAT && dynamic_cast<BulletStrategy*>((*iter)->getStrat())->getSource().compare(g->getName())==0))
                {
                        // nothing
                }
                else {
                    g->setMoveDelta(Vector2f(0,0));
	            GameEvents::EventQueue::getInstance().push(new GameEvents::CollideEvent(g->getName(), (*iter)->getName(), g->getPosition()));
                }
	    }
	} 
    }
}

/* Checks if a position after the passed in hypothetical increase is within the grid
   If valid, return hypothetical position
   If invalid, return the valid movement
   The time measure, fticks, is passed by reference to allow it to be adjusted if the movement is cut short
   The boolean, collision, is passed by reference to let the caller know if the movement was cut short
*/
void MapManager::validateMovement(GridElement* g) const{

    Tile tile;
    Vector2f delta = g->getMoveDelta();
    std::vector<Vector2f> movebox = g->getMoveboxVertices();
    bool collision=false;

    //check each corner of the movebox
    for(int i=0; i<4; ++i){
        tile = findTileAt(movebox[i] + delta);
    	if( (tile.getName().compare(std::string("uninitialized tile")) ==0) ||
            (!tile.isCollidable())) {
            g->setMoveDelta(Vector2f(0,0));
	    collision = true;
	}
    }

    //check midpoints between corners
    float diffX = movebox[1][0] - movebox[0][0];  
    float diffY = movebox[2][1] - movebox[0][1];  
    tile = findTileAt(movebox[0] + Vector2f(diffX/2.,0) + delta);
    if( (tile.getName().compare(std::string("uninitialized tile")) ==0) ||
        (!tile.isCollidable())) {
        g->setMoveDelta(Vector2f(0,0));
        collision = true;
    }
    tile = findTileAt(movebox[0] + Vector2f(0, diffY/2.) + delta);
    if( (tile.getName().compare(std::string("uninitialized tile")) ==0) ||
        (!tile.isCollidable())) {
        g->setMoveDelta(Vector2f(0,0));
        collision = true;
    }    
    tile = findTileAt(movebox[0] + Vector2f(diffX,diffY/2.) + delta);
    if( (tile.getName().compare(std::string("uninitialized tile")) ==0) ||
        (!tile.isCollidable())) {
        g->setMoveDelta(Vector2f(0,0));
        collision = true;
    }
    tile = findTileAt(movebox[0] + Vector2f(diffX/2.,diffY) + delta);
    if( (tile.getName().compare(std::string("uninitialized tile")) ==0) ||
        (!tile.isCollidable())) {
        g->setMoveDelta(Vector2f(0,0));
        collision = true;
    }

    for(int i=0; i < mapWidth * mapHeight; ++i){
	collideGridEles(i,g);
    }

    // if we hit a collidable tile, fire off a collision event
    if(collision){
        tile = findTileAt(g->getPosition()+g->getMoveDelta());
        GameEvents::EventQueue::getInstance().push(new GameEvents::CollideEvent(g->getName(), tile.getName(), g->getPosition()));
    }

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
#endif

        (*it)->draw();

#ifdef HITBOX
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

    // unnecessary right now, our tiles don't do anything
    // update each tile in each layer
    /*for(std::list<std::vector<Tile>  >::const_iterator it = mapLayers.begin(); it!=mapLayers.end(); ++it)
    {   
        for(std::vector<Tile>::const_iterator layer_it = (*it).begin(); layer_it != (*it).end(); ++layer_it)
        {
            (*layer_it).update(ticks);
        }
    }*/

    // Add anything waiting in reserve whose creation timer has hit 0 to the map
    std::map<GridElement*, int>::iterator resIt = reserve.begin();
    while(resIt != reserve.end()){
        if((*resIt).second ==0)
        {
            addGridElement((*resIt).first);
            (*resIt).first->resetHP();
            (*resIt).first->getStrat()->unsuppress();
            std::map<GridElement*,int>::iterator toErase = resIt;
            resIt++;
            reserve.erase(toErase);
        }
        // if it's not time just tick down the timer
        else
        {
            (*resIt).second--;
            ++resIt;
        }
    }


    // Update only important tiles
    for(std::list<Tile*>::const_iterator it=updateTiles.begin(); it!= updateTiles.end(); ++it)
    {
        (*it)->update(ticks);
    }

    // This is a temp list used to store GridElements as we sort them by index
    // Here we just create it and set its size to be the same as the main list
    std::vector<std::list<GridElement *> > tempVec;
    tempVec.reserve(mapWidth*mapHeight);
    for(int i=0; i < mapWidth*mapHeight; i++)
    {   
        tempVec.push_back(std::list<GridElement*>());
    }

    // Walk through Grid elements, update and then find their new position, add to tempVec
    for(std::vector<std::list<GridElement*> >::iterator it=gridElements.begin(); it!=gridElements.end(); ++it){
        for(std::list<GridElement*>::iterator l_it = (*it).begin(); l_it != (*it).end(); ++l_it){

            // have element calculate its new position
	    (*l_it)->update(ticks);

            // then check if that's a legal position in the world
            validateMovement((*l_it));

            // now have the element actually move
            (*l_it)->applyMoveDelta();

            // put it into the temp vector at its assigned position
 	    //int index = getIndexAt((*l_it)->getMoveboxVertices()[3]);
            float diffX = gridToWorld((*l_it)->getMoveboxVertices()[2])[0] - gridToWorld((*l_it)->getMoveboxVertices()[3])[0];
 	    int index = getIndexAt((*l_it)->getMoveboxVertices()[3] + Vector2f(diffX/2, 0));

            try {tempVec[index].push_back(*l_it); }
            catch(const std::out_of_range& e) {
                std::cerr << "Tried to access GridElement at index " << index << std::endl;
            }
	}
    }

    // We've updated everything, clear out the main list
    for(std::vector<std::list<GridElement*> >::iterator it=gridElements.begin(); it!=gridElements.end(); ++it){
	(*it).clear();
    }

    // Copy the temporary list over
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

void MapManager::onDeath(const GameEvents::Event *e) {
    // Don't delete the player plz
    if(player->getName().compare(e->getSource()) == 0) {
        GameEvents::EventQueue::getInstance().push(new GameEvents::LoseEvent(e->getSource(), e->getPosition()));
        return;
    }
    removeGridElement(e->getSource());
}


void MapManager::onCreate(const GameEvents::Event *e) {
    const GameEvents::CreateEvent *c = dynamic_cast<const GameEvents::CreateEvent*>(e);
    GridElement* newGE;

    // Ignore creation events we pushed
    if(e->getSource().compare("map") ==0) 
        return; 
    
    // If the requested location is legal
    else if(getIndexAt(worldToGrid(e->getPosition()))>0)
    {
        // If create is requested immediately, add it directly to the map
        if(c->getTimer()==0)
        {
            addGridElement(newGE = new GridElement(c->getSprite(), worldToGrid(e->getPosition()), c->getDir(), c->getStrat(), c->getTarget()));

        }
        // otherwise store it until it's ready
        else {
            reserve.insert(std::pair<GridElement*, int>(newGE = new GridElement(c->getSprite(), worldToGrid(e->getPosition()), c->getDir(), c->getStrat(), c->getTarget()), c->getTimer()));
            newGE->getStrat()->suppress();
        }
        // set it up if it's a bullet XXX TODO THE MAP SHOULDN'T BE HANDLING THIS
        if(c->getStrat() == BULLET_STRAT) {
            dynamic_cast<BulletStrategy*>(newGE->getStrat())->setSource(c->getSource());
        }

        // push a creation notification
    GameEvents::EventQueue::getInstance().push(new GameEvents::CreateEvent("map", c->getSprite(), e->getPosition(), c->getDir(), c->getTarget(), c->getStrat()));
    }
}


/*********** Listener set up and forwarder ************/


// Forwarding function creation events
void MapCreateForwarder(Listener* context, const GameEvents::Event *e) {
    dynamic_cast<MapManager*>(context)->onCreate(e);
}

void MapDeathForwarder(Listener* context, const GameEvents::Event *e) {
    dynamic_cast<MapManager*>(context)->onDeath(e);
}

void MapManager::registerListeners() {
    GameEvents::EventQueue::getInstance().addListener(GameEvents::CREATE_EVENT, static_cast<Listener*>(this), &MapCreateForwarder);
    GameEvents::EventQueue::getInstance().addListener(GameEvents::DEATH_EVENT, static_cast<Listener*>(this), &MapDeathForwarder);
}
