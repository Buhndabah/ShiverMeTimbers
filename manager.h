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

enum startOver {
    YES=1,
    NO=2
};

class Manager : public Listener{
public:
  Manager ();
  ~Manager ();
  bool play();
  void reinit();

  virtual void onWin(const GameEvents::Event*);
  virtual void onLose(const GameEvents::Event*);
  virtual void onGameEnd(const GameEvents::Event*);

private:
  const bool env;
  const IOManager& io;
  Clock& clock;
  World world;
  Viewport& viewport;

  SDL_Surface * const screen;

  GridElement* player;
  MapManager& map;
  HUD& hud;

  bool gameOver;
  int restart;

  void draw() const;
  void update();

  virtual void registerListeners();

  Manager(const Manager&);
  Manager& operator=(const Manager&);
};

void ManagerWinForwarder(Listener*, const GameEvents::Event*);
void ManagerLoseForwarder(Listener*, const GameEvents::Event*);
void ManagerGameEndForwarder(Listener*, const GameEvents::Event*);
