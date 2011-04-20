/*
  Sprite
  
  Abstract Base Class for visible objects in PatternSpace.  A sprite needs to 
know it's current absolute position and rotation in space, and which image 
it's associated with.  Then it can draw itself onto a Screen.

  Sprite is basically a Mixin: by deriving from Sprite and implementing 
the virtual methods, you can easily give your class visibility.

*/
#ifndef PATTERN_SPACE_SPRITE_INCLUSION_GUARD
#define PATTERN_SPACE_SPRITE_INCLUSION_GUARD
#include "vector2d.h"
#include "image.h"

namespace PatternSpace {

    class Sprite {
        virtual double angle();
        virtual Vector2d position();
        virtual Image& image();
    public:
        void draw(Screen) {
            Vector2d screenPosition = position() - screen.origin();
            image().draw(screen, screenPosition, angle() );
        }
    }; // end class Sprite


} // end namespace PatternSpace

#endif


