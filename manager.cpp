#include <iostream>
#include <string>
#include <iomanip>
#include <math.h>
#include "multisprite.h"
#include "rotatesprite.h"
#include "gamedata.h"
#include "manager.h"

Manager::~Manager() { 
  // Manager made it, so Manager needs to delete it
  for(unsigned int i = 0; i < snowballs.size(); i++){
     delete snowballs[i]; 
  }
}

Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  world("back", Gamedata::getInstance().getXmlInt("backFactor") ),
  viewport(Viewport::getInstance() ),
  screen( io.getScreen() ),

  snowballs(),
  currentSprite(0),

  player(NULL),

  makeVideo( false ),
  frameCount( 0 ),
  username(  Gamedata::getInstance().getXmlStr("username") ),
  frameMax( Gamedata::getInstance().getXmlInt("frameMax") ),
  TITLE( Gamedata::getInstance().getXmlStr("screenTitle") )
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw std::string("Unable to initialize SDL: ");
  }
  atexit(SDL_Quit);
  int numSnowballs = Gamedata::getInstance().getXmlInt("numSnowballs");
  snowballs.reserve(numSnowballs+4);

  player = new GridElement("coolyeti");
//  snowballs.push_back(new MultiSprite("spinsnowball"));
  //snowballs.push_back(new MultiSprite("coolyeti"));
//  snowballs.push_back(player);
  //snowballs.push_back(new RotateSprite("yeti"));
  //snowballs.push_back(new MultiSprite("spinsnowball"));
  //snowballs.push_back(new RotateSprite("penguin"));
  for(int i=0; i<numSnowballs; ++i)
	snowballs.push_back(new Sprite("snowball"));
//  viewport.setObjectToTrack(snowballs[currentSprite]);
  viewport.setObjectToTrack(&player->getSprite());
//  viewport.setObjectToTrack(snowballs[1]);
}


void Manager::draw() const {
  world.draw();
  player->getSprite().draw();
  std::vector<Drawable*>::const_iterator it = snowballs.begin();
  while(it != snowballs.end()){
	(*it)->draw();
	++it;
  }
  io.printMessageCenteredAt(TITLE, 10);
  io.printMessageValueAt("fps: ", clock.getFps(), 10, 10);
//  io.printMessageAt("Controls: T to track next sprite", 10, 30);
//  io.printMessageAt("               R to rotate special sprites", 10, 50);
  io.printMessageAt("Stephen Wells", 500, 450);
  viewport.draw();

  SDL_Flip(screen);
}

void Manager::update() {
  ++clock;
  Uint32 ticks = clock.getElapsedTicks();
  player->update(ticks);
  std::vector<Drawable*>::iterator it = snowballs.begin();
  while(it != snowballs.end()){
	(*it)->update(ticks);
	++it;
  }

  if ( makeVideo && frameCount < frameMax ) {
    std::stringstream strm;
    strm << "video/" << username<< '.' 
         << std::setfill('0') << std::setw(4) 
         << frameCount++ << ".bmp";
    std::string filename( strm.str() );
    std::cout << "Making frame: " << filename << std::endl;
    SDL_SaveBMP(screen, filename.c_str());
  }
  world.update();
  viewport.update();	//update the viewport last
}

void Manager::play() {
  SDL_Event event;

  bool done = false;
  bool keyCatch = false;

  bool w,a,s,d;
  w = a = s = d = false;

  while ( not done ) {

std::cout << "display: " << player->getSprite().getPosition() << std::endl;
std::cout << "grid: " << player->getGridPosition() << std::endl << std::endl;
    //adjust the player's velocity according to the key(s) being held down
    if(w){
      if(a)
        player->moveUpLeft();
      else if(d)
        player->moveUpRight();
      else if(s)
        player->stop();
      else
        player->moveUp();
    }
    else if(s){
      if(a)
        player->moveDownLeft();
      else if(d)
        player->moveDownRight();
      else
        player->moveDown();
    }
    else if(a && !d)
      player->moveLeft();
    else if(d && !a)
      player->moveRight();
    else
      player->stop();

    SDL_PollEvent(&event);
    Uint8 *keystate = SDL_GetKeyState(NULL);

    if (event.type ==  SDL_QUIT) { done = true; break; }
    if(event.type == SDL_KEYUP) {
      //when a key is lifted, check to see if it was a movement button
      if(!keystate[SDLK_w]) w = false;
      if(!keystate[SDLK_a]) a = false;
      if(!keystate[SDLK_s]) s = false;
      if(!keystate[SDLK_d]) d = false;

      keyCatch = false;
    }
    if(event.type == SDL_KEYDOWN) {
      if (keystate[SDLK_ESCAPE]){// || keystate[SDLK_q]) {
        done = true;
        break;
      }

      if (keystate[SDLK_F4] && !makeVideo) {
        std::cout << "Making video frames" << std::endl;
        makeVideo = true;
      }

      if (keystate[SDLK_p] && !keyCatch) {
        keyCatch = true;
        if ( clock.isPaused() ) clock.unpause();
        else clock.pause();
      }

      //change tracking sprite
      if (keystate[SDLK_t] && !keyCatch) {
	keyCatch = true;
	if(currentSprite  < snowballs.size())
	  viewport.setObjectToTrack(snowballs[currentSprite++]);
	else{
          viewport.setObjectToTrack(&player->getSprite());
          currentSprite = 0;
        }
      }

      //rotate RotateSprite's
      if (keystate[SDLK_r] && !keyCatch) {
	keyCatch = true;
	Gamedata::getInstance().setRoto(!Gamedata::getInstance().getRoto());
      }

      //check for player movement input
      if (keystate[SDLK_w]){
        keyCatch = true;
        w = true;
      }
      if (keystate[SDLK_a]){
        keyCatch = true;
        a = true;
      }
      if (keystate[SDLK_s]){
        keyCatch = true;
        s = true;
      }
      if (keystate[SDLK_d]){
        keyCatch = true;
        d = true;
      }
    }

    draw();
    update();
  }
}
