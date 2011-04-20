/*
    SpriteMass
    
  SpriteMass "is a" Sprite and "has a" Mass.  Delegation is appropriate for
Mass because Mass can be subclassed and its interface should not be exposed
directly.

Inheritence is appropriate for Sprite because its interface (namely the draw()
method) needs to be exposed, and there is no other behavor that needs to be
controlled.
     
*/
#ifndef PATTERN_SPACE_SHIPS_INCLUSION_GUARD
#define PATTERN_SPACE_SHIPS_INCLUSION_GUARD

#include "vector2d.h"
#include "image.h"
#include "mass.h"

namespace PatternSpace {
    
    class SpriteMass : public Sprite {
    public:
        SpriteMass::SpriteMass(Image,Mass&);
        
        Image img;
        Mass& mass;
        virtual double angle();
        virtual Vector2d position();
        virtual Image& image();
        void step(double deltaTime);
    }; // end class SpriteMass
    
} // end namespace PatternSpace
#endif
