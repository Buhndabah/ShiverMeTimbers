//#define TEST_ENEMY

#include <ctime>
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
}

Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  io( IOManager::getInstance() ),
  clock(Clock::getInstance() ),
  world("back", Gamedata::getInstance().getXmlInt("backFactor") ),
  viewport(Viewport::getInstance() ),
  screen( io.getScreen() ),

  player(NULL),
  map(MapManager::getInstance()),
  hud(HUD::getInstance()),

  gameOver(false),
  restart(0) {

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) != 0) {
    throw std::string("Unable to initialize SDL: ");
  }

  atexit(SDL_Quit);

// init sound
  SoundManager::getInstance();

// Add player to map
  player = new GridElement("coolyeti"); // deleted by the mapManager
  map.addGridElement(player);
  map.setPlayer(player);
  hud.addHealthBar(player->getName(), Vector2f(0, -10));
  viewport.setObjectToTrack(&(player->getSprite()));
 
#ifdef TEST_ENEMY 
  // test target 
  GridElement* test;
  map.addGridElement(test = new GridElement("icecream", Vector2f(650,650), 0, TURRET_STRAT));
  hud.addHealthBar(test->getName(), Vector2f(0,-10));
#endif

// set up listeners
  registerListeners();
}


void Manager::draw() const {

// draw background, map, view, then hud
  world.draw();
  map.draw();
  viewport.draw();
  hud.draw();

// get all that on the screen
  SDL_Flip(screen);

}

void Manager::update() {

// tick up clock
  ++(clock);
  Uint32 ticks = clock.getElapsedTicks();

// send out events from queue
  GameEvents::EventQueue::getInstance().prepEvents();

// update all objects
  world.update();
  map.update(ticks);
  viewport.update();	//update the viewport last
  hud.update(ticks);

}


bool Manager::play() {
  SDL_Event event;

  bool done = false;
  bool keyCatch = false;

  bool w,a,s,d,space;
  w = a = s = d = space = false;

// go until told to quit
  while ( not done ) {

//std::cout << "display: " << player->getSprite().getPosition() << std::endl;
//std::cout << "grid: " << player->getGridPosition() << std::endl;


    // move the player depending on what keys are currently down
    
    // opposites - stop
    if( (w && s) || (a && d) ) { 
        player->stop();
    }
    if(w && a) {
        player->moveUpLeft();
    }
    else if(w && d) {
        player->moveUpRight();
    }
    else if(s && a) {
        player->moveDownLeft(); 
    }
    else if(s && d) {
        player->moveDownRight();
    }
    else if(w && s) {
        player->stop();
    }
    else if(a && !d) {
      player->moveLeft();
    }
    else if(d && !a) {
      player->moveRight();
    }
    else if(w) {
        player->moveUp();
    }
    else if(s) {
        player->moveDown();
    }
    // catch all
    else {
      player->stop();
    }

    // check whether we need to shoot
    if(space){

        // get coordinates from mouse
	int x, y;
	SDL_GetMouseState(&x,&y);
	Vector2f target(x,y);

        // apply viewport offset and shoot at it
	target += viewport.getPosition();
        player->shoot(target);
    }

    // Get input and set key flags

    while(SDL_PollEvent(&event)) {

      if (event.type ==  SDL_QUIT) { 
        done = true; 
        break; 
      }

      Uint8 *keystate = SDL_GetKeyState(NULL);

      //when a key is lifted, check to see if it was a movement button
      if(event.type == SDL_KEYUP) {
        if(!keystate[SDLK_w]&&!keystate[SDLK_UP]) w = false;
        if(!keystate[SDLK_a]&&!keystate[SDLK_LEFT]) a = false;
        if(!keystate[SDLK_s]&&!keystate[SDLK_DOWN]) s = false;
        if(!keystate[SDLK_d]&&!keystate[SDLK_RIGHT]) d = false;
        keyCatch = false;
      }

      // space being lifted
      if( event.type == SDL_KEYUP && !keystate[SDLK_SPACE])  {
        space = false;
        keyCatch = false;
      }

      // Handle mouse clicks
      if(event.type == SDL_MOUSEBUTTONDOWN) {
        if(event.button.button==SDL_BUTTON_LEFT)
        {
            int x,y;
            SDL_GetMouseState(&x,&y);
            GameEvents::EventQueue::getInstance().push(new GameEvents::ClickEvent("SDLMOUSE", Vector2f(x,y)));
        }
      }

      // Handle key presses
      if(event.type == SDL_KEYDOWN) {
            if (keystate[SDLK_ESCAPE]){// || keystate[SDLK_q]) {
                done = true;
                break;
            }

            // Turn the "help" screen on and off
            if(keystate[SDLK_F1] && !keyCatch) {
                keyCatch=true;
                hud.toggleHelp();
            }

            // Pause the game
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

            //XXX change tracking sprite
            //currently we don't track anything besides the player
            if (keystate[SDLK_t] && !keyCatch) {
	        keyCatch = true;
                viewport.setObjectToTrack(&player->getSprite());
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
        } // end key down
    } // end pollevents

    // Now draw and update
    draw();
    update();


    if(restart) break;
  }
  return restart == YES ? true : false;
}

// reset everything
void Manager::reinit() {
    GameEvents::EventQueue::getInstance().reinit();
    map.reinit();
    hud.reinit();
    player = new GridElement("coolyeti");
    map.addGridElement(player);
    map.setPlayer(player);
#ifdef TEST_ENEMY
    GridElement* test;
    map.addGridElement(test = new GridElement("icecream", Vector2f(650, 650),0, TURRET_STRAT));
    hud.addHealthBar(test->getName(), Vector2f(0,-10));
#endif
    hud.addHealthBar(player->getName(), Vector2f(0,-10));
    viewport.setObjectToTrack(&player->getSprite());
    registerListeners();
    gameOver = false;
    restart = 0;
    if ( clock.isPaused() ) 
    {
            clock.unpause();
    }
    GameEvents::EventQueue::getInstance().clearEvents();
}

/******** Event Handling Functions **********/

void Manager::onWin(const GameEvents::Event* e) {
    (void)e;
    hud.onWin();
    clock.pause();
    gameOver = true;
}

void Manager::onLose(const GameEvents::Event* e) {
    (void)e;
    hud.onLose();
    clock.pause();
    gameOver = true;
}

void Manager::onGameEnd(const GameEvents::Event* e) {
    if(static_cast<const GameEvents::GameEndEvent*>(e)->getRestart())
    {
        restart=YES;
    }
    else
    {
        restart=NO;
    }
}
void Manager::registerListeners() {
    GameEvents::EventQueue::getInstance().addListener(GameEvents::LOSE_EVENT, static_cast<Listener*>(this), &ManagerLoseForwarder);
    GameEvents::EventQueue::getInstance().addListener(GameEvents::WIN_EVENT, static_cast<Listener*>(this), &ManagerWinForwarder);
    GameEvents::EventQueue::getInstance().addListener(GameEvents::GAMEEND_EVENT, static_cast<Listener*>(this), &ManagerGameEndForwarder);
}

void ManagerWinForwarder(Listener* context, const GameEvents::Event *e) {
    dynamic_cast<Manager*>(context)->onWin(e);
}
void ManagerLoseForwarder(Listener* context, const GameEvents::Event *e) {
    dynamic_cast<Manager*>(context)->onLose(e);
}
void ManagerGameEndForwarder(Listener* context, const GameEvents::Event *e) {
    dynamic_cast<Manager*>(context)->onGameEnd(e);
}
