#include <SDL.h>
#include <vector>
#include "ioManager.h"
#include "clock.h"
#include "sprite.h"
#include "world.h"
#include "viewport.h"
#include "player.h"

class Manager {
public:
  Manager ();
  ~Manager ();
  void play();

private:
  const bool env;
  const IOManager& io;
  Clock& clock;
  World world;
  Viewport& viewport;

  SDL_Surface * const screen;

  std::vector<Drawable *> snowballs;
  unsigned int currentSprite;

  Player *player;

  bool makeVideo;
  int frameCount;
  std::string username;
  int frameMax;
  const std::string TITLE;

  void draw() const;
  void update();

  Manager(const Manager&);
  Manager& operator=(const Manager&);
};
