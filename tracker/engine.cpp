#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include <ctime>
#include "sprite.h"
#include "multisprite.h"
#include "twowaysprite.h"
#include "smartSprite.h"
#include "subjectSprite.h"
#include "player.h"
#include "gameData.h"
#include "engine.h"
#include "frameGenerator.h"
#include "collisionStrategy.h"

Engine::~Engine() { 
  for(auto& t : dumb)
  {
  	delete t;
  }
  for(auto& t : dumbSmall)
  {
  	delete t;
  }
  for(auto& t : smart)
  {
  	delete t;
  }
  for(auto& c : strategies)
  {
  	delete c;
  }
  
  delete player;
  std::cout << "Terminating program" << std::endl;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IoMod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc.getRenderer() ),
  ocean("back", Gamedata::getInstance().getXmlInt("back/factor") ),
  mount("mountains", Gamedata::getInstance().getXmlInt("mountains/factor")),
  viewport( Viewport::getInstance() ),
  player(new SubjectSprite("shark")),
  dumb(),
  dumbSmall(),
  smart(),
  currentStrategy(0),
  collisionCount(0),
  collision(false),
  strategies(),
  h(hud::getInstance()),
  hudState(true),
  makeVideo( false ),
  sound(),
  finish(false),
  godMode(false)
  //lights()
{
  
  strategies.push_back( new PerPixelCollisionStrategy );
  strategies.push_back( new RectangularCollisionStrategy );
  strategies.push_back( new MidPointCollisionStrategy );
  
  player->setScale(1.5);
  int a = Gamedata::getInstance().getXmlInt("numberOfFish1");
  int b = Gamedata::getInstance().getXmlInt("numberOfStarfish");
  int c = Gamedata::getInstance().getXmlInt("numberOfTrash");
  int d = Gamedata::getInstance().getXmlInt("numberOfSmallFish");
  dumb.reserve(a+b);
  smart.reserve(c);
  dumbSmall.reserve(d);
  
  for(int i = 0; i < a; i++)
  {
  	dumb.push_back(new Sprite("fish1"));
  	dumb[i]->setScale(.4);
  }
  
  for(int i = 0; i < d; i++)
  {
  	if(i % 2 == 0){
  		dumbSmall.push_back(new Sprite("smallfish1"));
  		dumbSmall[i]->setScale(.15);
  	}
  	else
  	{
  		dumbSmall.push_back(new Sprite("smallstarfish"));
  		dumbSmall[i]->setScale(.15);
  	}
  	
  }
  
  for(int i = 0; i < b; i++)
  {
  	dumb.push_back(new Sprite("starfish"));
  	dumb[i+a]->setScale(.4);
  }
  
  Vector2f pos = player->getPosition();
  int w = player->getScaledWidth();
  int h = player->getScaledHeight();
  
  for(int i = 0; i < c; i++)
  {
  	smart.push_back(new SmartSprite("trash", pos, w, h));
  	player->attach(smart[i]);
  	smart[i]->setScale(.6);
  }
  
  
  
   
  Viewport::getInstance().setObjectToTrack(player);
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  ocean.draw();

  for(auto& t : dumbSmall)
  {
  	t->draw();
  }
    
  mount.draw();
  h.draw(hudState, 2-collisionCount, smart.size());
  player->draw();
  
  //strategies[currentStrategy]->draw();

  for(auto& t : dumb)
  {
  	t->draw();
  }
  
  for(auto& t : smart)
  {
  	t->draw();
  }
  
  if(collisionCount == 2 && finish){
  	SDL_Rect screen;
  	screen.x = 0;
  	screen.y = 0;
  	screen.w = 900;
  	screen.h = 680;
  	
  	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor( renderer, 255, 255, 255, 175);
    SDL_RenderFillRect(renderer, &screen);
    
    io.writeText("You lost! Press R to restart the game or ESC/q to quit", 170, 300);
    
  	clock.pause();
  	
  }
  else if(collisionCount != 2  && smart.size() == 0)
  {
    
  	SDL_Rect screen;
  	screen.x = 0;
  	screen.y = 0;
  	screen.w = 900;
  	screen.h = 680;
  	
  	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor( renderer, 255, 255, 255, 175);
    SDL_RenderFillRect(renderer, &screen);
    
    io.writeText("Congratulations! You won!", 320, 280);
    io.writeText("Press R to restart the game or ESC/q to quit", 240, 340);

    clock.pause();
  }
  
  //lights.draw();
  viewport.draw();
  SDL_RenderPresent(renderer);
}

void Engine::checkForCollisions() {
  
  //Detection of contact collisions of trash sprites
  auto it = smart.begin();
  while ( it != smart.end() ) {
    if ( strategies[currentStrategy]->execute(*player, **it) ) {
      SmartSprite* doa = *it;
      player->detach(doa);
      delete doa;
      it = smart.erase(it);
    }
    else ++it;
  }
  
  //Detecting shooting collisions
  auto t = smart.begin();
  while ( t != smart.end() ) {
    if ( player->shot(*t)) {
      SmartSprite* doa = *t;
      player->detach(doa);
      delete doa;
      t = smart.erase(t);
    }
    else ++t;
  }
  
  //Detecting fish/shark collisions
  if(!godMode){
  	auto u = dumb.begin();
  	while (u != dumb.end()){
  		if(player->collidedWith(*u)){
  		  player->explode();
  		  collisionCount++;
  		}
  		else ++u;
  	}
  }
  
}

void Engine::update(Uint32 ticks) {
  checkForCollisions();
  ocean.update();
  
  for(auto& t : dumbSmall)
  {
  	t->update(ticks);
  }
    
  mount.update();
  player->update(ticks);
  
  if(player->explosionDone())
  {
  	finish = true;
  }

  
  for(auto& t : dumb)
  {
  	t->update(ticks);
  }
  
  for(auto& t : smart)
  {
  	t->update(ticks);
  }
  //lights.update();
  viewport.update(); // always update viewport last
}


bool Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  while ( !done ) {
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_F1]){
          if(hudState == 0) hudState = 1;
          else
          	hudState = 0;
        }
        if ( keystate[SDL_SCANCODE_O] ) {
          sound.toggleMusic();
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if ( keystate[SDL_SCANCODE_M] ) {
          currentStrategy = (1 + currentStrategy) % strategies.size();
        }
        if(keystate[SDL_SCANCODE_R]){
        	clock.unpause();
        	return true;
        }
        if(keystate[SDL_SCANCODE_G]){
        	godMode = true;
        }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
      }
    }

    // In this section of the event loop we allow key bounce:

    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
      if (keystate[SDL_SCANCODE_SPACE]){
      	static_cast<Player*>(player)->shoot();
      	sound[0];
      }
      if (keystate[SDL_SCANCODE_A]) {
        static_cast<Player*>(player)->left();
      }
      if (keystate[SDL_SCANCODE_D]) {
        static_cast<Player*>(player)->right();
      }
      if (keystate[SDL_SCANCODE_W]) {
        static_cast<Player*>(player)->up();
      }
      if (keystate[SDL_SCANCODE_S]) {
        static_cast<Player*>(player)->down();
      }
      checkForCollisions();
      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
  return false;
}
