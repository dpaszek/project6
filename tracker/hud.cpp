#include <string>
#include <sstream>
#include <ctime>
#include "hud.h"	
#include "gameData.h"
#include "renderContext.h"

void writeText(TTF_Font* v, const std::string& msg, int x, int y, SDL_Color textColor, 
	SDL_Renderer* r)
{
	SDL_Surface* surface = TTF_RenderText_Solid(v, msg.c_str(), textColor);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(r, surface);

    int textWidth = surface->w;
    int textHeight = surface->h;
    SDL_FreeSurface(surface);
    SDL_Rect dst = {x, y, textWidth, textHeight};

    SDL_RenderCopy(r, texture, NULL, &dst);
    SDL_DestroyTexture(texture);
}

hud& hud::getInstance()
{
	static hud instance;
	return instance;
}

hud::~hud()
{
	TTF_CloseFont(font);
	TTF_Quit();
}

hud::hud() : 
	r(Gamedata::getInstance().getXmlInt("hud/r")),
	g(Gamedata::getInstance().getXmlInt("hud/g")),
	b(Gamedata::getInstance().getXmlInt("hud/b")),
	a(Gamedata::getInstance().getXmlInt("hud/a")),
	font(TTF_OpenFont(Gamedata::getInstance().getXmlStr("font/file").c_str(),
					  Gamedata::getInstance().getXmlInt("hud/size"))),
	init(TTF_Init()),
	rend(RenderContext::getInstance().getRenderer()),
	clock(Clock::getInstance())
{}

void hud::draw(bool enable, int lives, int sprites) const
{
	if(enable == true)
	{
		SDL_Rect re;
    	re.x = 0;
    	re.y = 0;
    	re.w = 370;
    	re.h = 210;
    	
    	SDL_SetRenderDrawColor( rend, r, g, b, a);
    	SDL_RenderDrawRect( rend, &re );
    	
    	SDL_Color col = {r, g, b, a};
    	std::string fps;
  		std::stringstream strm;
  		strm << "fps: " << clock.getFps();
  		fps = strm.str();
  		string msg1 = "Objective: Clean ocean by eating/shooting trash";
  		writeText(font, msg1, 5, 5, col, rend);
  		string msg1l2 = "Objective: Avoid other fish! Fish are friends, not food!";
  		writeText(font, msg1l2, 5, 25, col, rend);
  		string msg2 = "ASDW - Move Shark Player";
  		writeText(font, msg2, 5, 50, col, rend);
  		string msg2l2 = "SPACE Bar - Shoot";
  		writeText(font, msg2l2, 5, 70, col, rend);
  		string msg3 = "P - Pause Game";
  		writeText(font, msg3, 5, 90, col, rend);
  		string msg4 = "ESC/q - Quit Game";
  		writeText(font, msg4, 5, 110, col, rend);
  		string msg5 = "F1 - Toggle HUD";
  		writeText(font, msg5, 5, 130, col, rend);
  		string msg6 = "G - God Mode";
  		writeText(font, msg6, 5, 150, col, rend);
  		string msg7 = "N - Night Mode";
  		writeText(font, msg7, 5, 170, col, rend);
  		writeText(font, fps, 5, 190, col, rend);
  		
  		
  		
  		SDL_Rect pool;
  		pool.x = 700;
  		pool.y = 0;
  		pool.w = 200;
  		pool.h = 60;
  		SDL_RenderDrawRect(rend, &pool);
  		
  		std::stringstream strm2;
  		strm2 << "Sprites Remaining: " << sprites;
  		std::string bCT = strm2.str();
  		
  		std::stringstream strm3;
  		strm3 << "Player Lives Remaining: " << lives;
  		std::string fCT = strm3.str();
  		
  		writeText(font, bCT, 705, 5, col, rend);
  		writeText(font, fCT, 705, 30, col, rend);

	}
}

