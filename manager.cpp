#include <iostream>
#include <string>
#include <iomanip>
#include <math.h>
#include "soundManager.h"
#include "multisprite.h"
#include "rotatesprite.h"
#include "gamedata.h"
#include "gameEvents.h"
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
  map(MapManager::getInstance()),
  hud(HUD::getInstance()),

  makeVideo( false ),
  frameCount( 0 ),
  username(  Gamedata::getInstance().getXmlStr("username") ),
  frameMax( Gamedata::getInstance().getXmlInt("frameMax") ),
  TITLE( Gamedata::getInstance().getXmlStr("screenTitle") ),
  gameOver(false)
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw std::string("Unable to initialize SDL: ");
  }

  // trap cursor in window
  SDL_WM_GrabInput( SDL_GRAB_ON );

  atexit(SDL_Quit);
  int numSnowballs = Gamedata::getInstance().getXmlInt("numSnowballs");
  snowballs.reserve(numSnowballs+4);

  player = new GridElement("coolyeti"); // deleted by the mapManager
  map.addGridElement(player);
  map.setPlayer(player);
  //map.addGridElement(new GridElement("icecream", CHASE_STRAT));
  
  // test target 
  GridElement* test;
  map.addGridElement(test = new GridElement("icecream", Vector2f(650,650), 0, TURRET_STRAT));
  hud.addHealthBar(test->getName(), Vector2f(0,-10));

  hud.addHealthBar(player->getName(), Vector2f(0, -10));
  SoundManager::getInstance();
//  snowballs.push_back(new MultiSprite("spinsnowball"));
  //snowballs.push_back(new MultiSprite("coolyeti"));
//  snowballs.push_back(player);
  //snowballs.push_back(new RotateSprite("yeti"));
  //snowballs.push_back(new MultiSprite("spinsnowball"));
  //snowballs.push_back(new RotateSprite("penguin"));
  //for(int i=0; i<numSnowballs; ++i)
	//snowballs.push_back(new Sprite("snowball"));
  viewport.setObjectToTrack(&(player->getSprite()));
//  viewport.setObjectToTrack(snowballs[1]);
  registerListeners();
}


void Manager::draw() const {
  Uint32 backColor = SDL_MapRGB(screen->format, 100,100,100);
  SDL_FillRect(screen, NULL, backColor);
  world.draw();
  map.draw();
  //player->draw();
  std::vector<Drawable*>::const_iterator it = snowballs.begin();
  while(it != snowballs.end()){
	(*it)->draw();
	++it;
  }
  //io.printMessageCenteredAt(TITLE, 10);
//  io.printMessageAt("Controls: T to track next sprite", 10, 30);
//  io.printMessageAt("               R to rotate special sprites", 10, 50);
  viewport.draw();
  hud.draw();
  SDL_Flip(screen);
}

void Manager::update() {
  ++clock;
  Uint32 ticks = clock.getElapsedTicks();
  //player->update(ticks);
  GameEvents::EventQueue::getInstance().prepEvents();
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
  map.update(ticks);
  viewport.update();	//update the viewport last
  hud.update(ticks);
}

void Manager::play() {
  SDL_Event event;

  bool done = false;
  bool keyCatch = false;

  bool w,a,s,d,space,shot;
  w = a = s = d = space = shot = false;

  while ( not done ) {

//std::cout << "display: " << player->getSprite().getPosition() << std::endl;
//std::cout << "grid: " << player->getGridPosition() << std::endl;


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

    // check for shots fired
    if(space&&!shot){
        player->shoot();
        shot = true;
    }

    if (event.type ==  SDL_QUIT) { done = true; break; }
    if(event.type == SDL_KEYUP) {
      //when a key is lifted, check to see if it was a movement button
      if(!keystate[SDLK_w]&&!keystate[SDLK_UP]) w = false;
      if(!keystate[SDLK_a]&&!keystate[SDLK_LEFT]) a = false;
      if(!keystate[SDLK_s]&&!keystate[SDLK_DOWN]) s = false;
      if(!keystate[SDLK_d]&&!keystate[SDLK_RIGHT]) d = false;
      if(!keystate[SDLK_SPACE]) { space = false; shot = false; }
      keyCatch = false;
    }
    if(event.type == SDL_MOUSEBUTTONUP) {
        if(event.button.button==SDL_BUTTON_LEFT) {
            space = false;
            shot = false;
        }
    }
    if(event.type == SDL_MOUSEBUTTONDOWN) {
        if(event.button.button==SDL_BUTTON_LEFT)
        {
            std::cerr<<"received mouse event" << std::endl;
            space = true;
        }
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

      if(keystate[SDLK_F1] && !keyCatch) {
          keyCatch=true;
          hud.toggleHelp();
      }

      if (keystate[SDLK_p] && !keyCatch &&!gameOver) {
        keyCatch = true;
        if ( clock.isPaused() ) 
        {
            hud.onPause(0);
            clock.unpause();
        }
        else { 
            hud.onPause(1);
            clock.pause();
        }
      }

      // XXX damage testing
      if(keystate[SDLK_r] && !keyCatch) {
          keyCatch= true;
          //player->onDamage(10);
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
      if (keystate[SDLK_w]||keystate[SDLK_UP]){
        SoundManager::getInstance()[0];
        w = true;
      }
      if (keystate[SDLK_a]||keystate[SDLK_LEFT]){
        SoundManager::getInstance()[0];
        a = true;
      }
      if (keystate[SDLK_s]||keystate[SDLK_DOWN]){
        SoundManager::getInstance()[0];
        s = true;
      }
      if (keystate[SDLK_d]||keystate[SDLK_RIGHT]){
        SoundManager::getInstance()[0];
        d = true;
      }
      if (keystate[SDLK_SPACE]){
          space = true;
      }
    }

    draw();
    update();
  }
}

/******** Event Handling Functions **********/

void Manager::onWin(const GameEvents::Event* e) {
    hud.onWin();
    clock.pause();
    gameOver = true;
}

void Manager::registerListeners() {
    GameEvents::EventQueue::getInstance().addListener(GameEvents::WIN_EVENT, static_cast<Listener*>(this), &ManagerWinForwarder);
}

void ManagerWinForwarder(Listener* context, const GameEvents::Event *e) {
    dynamic_cast<Manager*>(context)->onWin(e);
}
