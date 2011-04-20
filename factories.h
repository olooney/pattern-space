/* Factories
  functions for creating new Solids.
*/

#ifndef PATTERN_SPACE_FACTORIES_INCLUSION_GUARD
#define PATTERN_SPACE_FACTORIES_INCLUSION_GUARD

#include <boost/shared_ptr.hpp>
#include "vector2d.h"
#include "solid.h"
#include "ship.h"

namespace PatternSpace {

    boost::shared_ptr<Solid> newRock(Vector2d initialPosition, Vector2d initialVelocity);
    boost::shared_ptr<Solid> newBigRock(Vector2d initialPosition, Vector2d initialVelocity);
    boost::shared_ptr<Solid> newAlien(Vector2d initialPosition, Vector2d initialVelocity);
    boost::shared_ptr<Solid> newExplosion(Vector2d initialPosition, Vector2d intialVelocity);
    boost::shared_ptr<Ship> newShip(Vector2d initialPosition, Vector2d initialVelocity);
    boost::shared_ptr<Solid> newMissle(Vector2d initialPosition, Vector2d intialVelocity);

} // end namespace PatternSpace


#endif  // PATTERN_SPACE_FACTORIES_INCLUSION_GUARD
