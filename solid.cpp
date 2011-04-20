/*
  Implementation for NormalSolid
  
  NormalSolid mostly just delegates, so is mostly defined inline in solid.h.
The new behavior defined here keeps track of age and damage.

*/

#include "solid.h"
#include "universe.h"
#include "factories.h"

namespace PatternSpace {
    
/*********************  NormalSolid  *********************/
    void NormalSolid::die() { dead = true; }
    
    Mass& NormalSolid::hit(const Vector2d impulse)
    { 
        damage += impulse.magnitude();
        if (hitPoints && damage > hitPoints) die();
        return pMass->hit(impulse);
    }

    void NormalSolid::step(double deltaTime)
    { 
        age += 1;
        if (life && (age>life) ) die();
        pMass->step(deltaTime);
    }
        
} // end namespace PatternSpace
