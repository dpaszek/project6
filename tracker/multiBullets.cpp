#include <iostream>
#include <cmath>
#include <vector>
#include "multiBullets.h"
#include "imageFactory.h"
#include "collisionStrategy.h"

multiBullets::~multiBullets(){
	delete strategy;
}


multiBullets::multiBullets(const std::string& n) :
  name(n),
  myVelocity(
    Gamedata::getInstance().getXmlInt(name+"/speed/x"), 
    Gamedata::getInstance().getXmlInt(name+"/speed/y")
  ),
  bulletImages(ImageFactory::getInstance().getImages(name)),
  bulletList(),
  freeList(),
  strategy(NULL)
{ 
  const string thisStrategy = 
    Gamedata::getInstance().getXmlStr("collisionStrategy");
  if ( thisStrategy == "PerPixel" ) {
    strategy = new PerPixelCollisionStrategy;
  }
  else if ( thisStrategy == "Rectangular" ) {
    strategy = new RectangularCollisionStrategy;
  }
  else if ( thisStrategy == "MidPoint" ) {
    strategy = new MidPointCollisionStrategy;
  }
}

multiBullets::multiBullets(const multiBullets& b) :
  name(b.name),
  myVelocity(b.myVelocity),
  bulletImages(b.bulletImages),
  bulletList(b.bulletList),
  freeList(b.freeList),
  strategy(b.strategy)
{}

void multiBullets::shoot(const Vector2f& pos, const Vector2f& objVel) {
  if(freeList.empty()){
    multiBullet b( name,  pos, objVel );
    bulletList.push_back( b );
  }
  else {
    multiBullet b = freeList.front();
    freeList.pop_front();
    b.reset();
    b.setVelocity(objVel);
    b.setPosition(pos);
    bulletList.push_back(b);
    
    
  }
}

bool multiBullets::collidedWith(const Drawable* obj) const { 
  for ( const auto & bullet : bulletList ) {
    if ( strategy->execute(bullet, *obj) ) {
      return true;
    }
  }
  return false;
}

void multiBullets::draw() const { 
  for ( const auto& bullet : bulletList ) {
    bullet.draw();
  }
}

void multiBullets::update(int ticks) { 
  std::list<multiBullet>::iterator ptr = bulletList.begin();
  while (ptr != bulletList.end()) {
    ptr->update(ticks);
    if (ptr->goneTooFar()) {  // Check to see if we should free a chunk
      freeList.push_back(*ptr);
      ptr = bulletList.erase(ptr);
    }   
    else ++ptr;
  }
}
