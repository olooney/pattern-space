/*
  Implementation for SpriteMass
*/

#include "ships.h"
#include "vector2d.h"
#include "image.h"
#include "mass.h"

namespace PatternSpace {
    
/*********************  SpriteMass  *********************/

// simply bind the image to the mass!
    Vector2d SpriteMass::position() { return mass.position(); }
    double SpriteMass::angle() { return mass.angle(); }
    Image& SpriteMass::image() { return img; }

    SpriteMass::SpriteMass(Image i, Mass& m):
        img(i), mass(m) {}
        
    void SpriteMass::step(double dT) {
        mass.step(dT);
    }
    
} // end namespace PatternSpace
