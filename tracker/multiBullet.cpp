#include <iostream>
#include <cmath>
#include "multiBullet.h"
#include "gameData.h"
#include "imageFactory.h"

multiBullet::multiBullet( const string& name, const Vector2f&  pos,
                          const Vector2f& vel) :
   Sprite(name, pos, vel, ImageFactory::getInstance().getImage(name)),
   distance(0), 
   maxDistance(Gamedata::getInstance().getXmlInt(name+"/distance")), 
   tooFar(false) 
{ }

void multiBullet::reset() {
  tooFar = false;
  distance = 0;
}

void multiBullet::setVelocity(const Vector2f& vel){
  Drawable::setVelocity(vel);
}

void multiBullet::setPosition(const Vector2f& pos){
  Drawable::setPosition(pos);
}

void multiBullet::update(Uint32 ticks) { 
  Vector2f pos = getPosition();
  Sprite::update(ticks);
  if ( getY()+getScaledHeight() < 0 || getY() > worldHeight ) {
    tooFar = true;
  }
  if ( getX() < 0 || getX()+getScaledWidth() > worldWidth ) {
    tooFar = true;
  }  
  distance += ( hypot(getX()-pos[0], getY()-pos[1]) );
  if (distance > maxDistance) tooFar = true;
}

