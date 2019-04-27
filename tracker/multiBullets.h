#ifndef MULTIBULLETS__H
#define MULTIBULLETS__H

#include <list>
#include <vector>
#include "vector2f.h"
#include "drawable.h"
#include "multiBullet.h"

class CollisionStrategy;
class multiBullets {
  multiBullets& operator=(const multiBullets&) = delete;
public:
  multiBullets(const std::string&);
  multiBullets(const multiBullets&);
  
  ~multiBullets();
  void draw() const;
  void update(int ticks);
  void shoot(const Vector2f& pos, const Vector2f& objVel);
  
  unsigned int bulletCount() const {return bulletList.size();}
  unsigned int freeCount() const {return freeList.size();}

  bool shooting() const { return !bulletList.empty(); }
  bool collidedWith(const Drawable*) const;

private:
  std::string name;
  Vector2f myVelocity;
  std::vector<Image*> bulletImages;
  std::list<multiBullet> bulletList;
  std::list<multiBullet> freeList;
  CollisionStrategy* strategy;
};
#endif
