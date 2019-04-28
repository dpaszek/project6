#ifndef HUD_H
#define HUD_H
#include <SDL_image.h>
#include <SDL.h>
#include "ioMod.h"
#include "clock.h"
#include "gameData.h"
#include "multiBullets.h"

class hud
{

public:
	hud();
	~hud();
	void draw(bool, int, int) const;
	static hud& getInstance();
	
	hud(const hud&) = delete;
	hud& operator=(const hud&) = delete;
private:
	unsigned char r;
	unsigned char g;
	unsigned char b; 
	unsigned char a;
	TTF_Font* font;
	int init;
	SDL_Renderer* rend;
	Clock& clock;
};

#endif

