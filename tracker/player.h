#ifndef PLAYER__H
#define PLAYER__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"
#include "multiBullets.h"

class CollisionStrategy;
class ExplodingSprite;

class Player : public Drawable {
public:
  Player(const std::string&);
  Player(const Player&);
  
  virtual ~Player();
  
  virtual void shoot();
  virtual bool shot(const Drawable*);
  virtual void explode();
  virtual bool collidedWith(const Drawable*);

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  virtual const Image* getImage() const { 
    return images[currentFrame]; 
  }
  int getScaledWidth()  const { 
    return getScale()*images[currentFrame]->getWidth(); 
  } 
  int getScaledHeight()  const { 
    return getScale()*images[currentFrame]->getHeight(); 
  } 
  virtual const SDL_Surface* getSurface() const { 
    return images[currentFrame]->getSurface();
  }

  void right();
  void left();
  void up();
  void down();
  void stop();
  
  bool explosionDone() const;
  
  unsigned int bulletCount() const {return bullets.bulletCount();}
  unsigned int freeCount() const {return bullets.freeCount();}
  unsigned int explosionCount() const {return collisions;}
  
  Player& operator=(const Player&) = delete;

private:
  std::vector<Image *> imagesRight;
  std::vector<Image *> imagesLeft;
  std::vector<Image *> images;

  //player attributes
  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;

  Vector2f initialVelocity;
  
  CollisionStrategy* collisionStrategy;
  ExplodingSprite* explosion;

  //bullet attributes
  std::string bulletName;
  float bulletInterval;
  float timeSinceLastBullet;
  float bulletSpeed;
  multiBullets bullets;
  int collisions;
  
  
  void advanceFrame(Uint32 ticks);
};
#endif
