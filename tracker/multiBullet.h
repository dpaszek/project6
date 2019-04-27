#ifndef MULTIBULLET__H
#define MULTIBULLET__H

#include "sprite.h"

class multiBullet : public Sprite {
public:
  multiBullet( const std::string&, const Vector2f&, const Vector2f& );
  virtual void update(Uint32 ticks);
  bool goneTooFar() const { return tooFar; }
  void reset();
  
  void setVelocity(const Vector2f&);
  void setPosition(const Vector2f&);

private:
  float distance;
  float maxDistance;
  bool tooFar;
};
#endif
