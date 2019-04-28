#include <list>
#include <SDL.h>
#include "triple.h"
#include "light.h"

class Gamedata;

class Lights {
public:
  Lights();
  ~Lights();

  void draw() const;
  void update() const;
  void addLights();
  void addLight(const Triple& triple, int brightness);
private:
  Gamedata& gdata;
  SDL_Renderer* renderer;
  unsigned int viewWidth;
  unsigned int viewHeight;
  unsigned int diffuseGridX;
  unsigned int diffuseGridY;
  std::list<Light*> lights;
  Lights(const Lights&);
  Lights& operator=(const Lights&);
};
