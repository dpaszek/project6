#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "subjectSprite.h"
#include "smartSprite.h"
#include "world.h"
#include "viewport.h"
#include "hud.h"
#include "sound.h"

class CollisionStrategy;
class SmartSprite;
class SubjectSprite;

class Engine {
public:
  Engine ();
  ~Engine ();
  void play();

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
private:
  const RenderContext& rc;
  const IoMod& io;
  Clock& clock;

  SDL_Renderer * const renderer;
  World ocean;
  World mount;
  Viewport& viewport;

  SubjectSprite* player;
  
  std::vector<Drawable*> dumb;
  std::vector<SmartSprite*> smart;
  
  int currentStrategy;
  bool collision;
  std::vector<CollisionStrategy*> strategies;
  
  const hud& h;
  bool hudState;
  
  bool makeVideo;
  SDLSound sound;

  void draw() const;
  void update(Uint32);

  void printScales() const;
  void checkForCollisions();
};
