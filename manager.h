#include <SDL.h>
#include <vector>
#include "listener.h"
#include "ioManager.h"
#include "clock.h"
#include "sprite.h"
#include "world.h"
#include "viewport.h"
#include "gridElement.h"
#include "mapManager.h"
#include "hud.h"

class Manager : public Listener{
public:
  Manager ();
  ~Manager ();
  void play();

  virtual void onWin(const GameEvents::Event*);

private:
  const bool env;
  const IOManager& io;
  Clock& clock;
  World world;
  Viewport& viewport;

  SDL_Surface * const screen;

  std::vector<Drawable *> snowballs;
  unsigned int currentSprite;

  GridElement *player;
  MapManager& map;
  HUD& hud;

  bool makeVideo;
  int frameCount;
  std::string username;
  int frameMax;
  const std::string TITLE;
  bool gameOver;

  void draw() const;
  void update();


  virtual void registerListeners();

  Manager(const Manager&);
  Manager& operator=(const Manager&);
};

void ManagerWinForwarder(Listener*, const GameEvents::Event*);
