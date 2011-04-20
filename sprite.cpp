/*
   Implementation for Sprite
*/

#inlcude "sprite.h"
#include "image.h"
#include "vector2d.h"

namespace PatternSpace {
/*********************  Sprite  *********************/

    void Sprite::draw(Screen screen) {
        Vector2d screenPosition = position() - screen.origin();
        image().draw(screen, screenPosition, angle() );
    }
} // end namespace PatternSpace
