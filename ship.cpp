/*
  Implementation for Ship class.
  
  Note that the Controls class, also defined in ship.h, is basically a struct
and has no implementation here.

  All we're doing is keeping track of the state of the relevant control keys,
and applying forces to the Ship based on which combination of keys is being
held down.

  TODO: The missle firing code here seems out of place and should be moved 
somewhere more appropriate.

*/

#include "ship.h"
#include "factories.h"

namespace PatternSpace {
/*********************  Ship  *********************/

   const double Ship::ENGINE_THRUST = .1;
   const double Ship::ENGINE_REVERSE_THRUST = .03;
   const double Ship::TURN_THRUST = 10;

    void Ship::step(double deltaTime) 
    {
        // apply the forces from the user controls
        if (upState) {
            Vector2d thrust = ENGINE_THRUST * Vector2d( angle() );
            push(thrust);
        }
        if (downState) {
            Vector2d thrust = - ENGINE_REVERSE_THRUST * Vector2d( angle() );
            push(thrust);
        }
        if (leftState) {
            torque( - TURN_THRUST );
        }
        if (rightState) {
            torque( TURN_THRUST );
        }
        
        // then let the base class take care of the rest.
        this->NormalSolid::step(deltaTime);            
    }

    // these functions inform the Ship about changes in the state of the
    // user controls.  Here, we simply record the current state.
    void Ship::up(bool state)   { upState = state; }
    void Ship::down(bool state) { downState = state; }
    void Ship::left(bool state) { leftState = state; }
    void Ship::right(bool state) { rightState = state; }
    
    // when the primary key is depressed, fire a single missle.
    void Ship::primary(bool state) 
    {
        if (state == true) {
            fireMissle = true;
        }
        primaryState = state;
    }
    
    // the next two functions form a kind of iterator: hasSpawn will be true
    // as long as there is a missle to fire, and nextSpawn will keep being
    // called until hasSpawn becomes false.  In this case, we set the internal
    // flag fireMissle when the user presses the fire key, and clear it after
    // firing a single missle.  
    bool Ship::hasSpawn() const {
        return fireMissle;
    }
    
    boost::shared_ptr<Solid> Ship::nextSpawn() {
        fireMissle = false;
        Vector2d forward = Vector2d( angle() );
        Vector2d misslePosition = position() + ( (radius()+6) * forward );
        Vector2d missleVelocity = velocity() + ( .8  * forward );
        return newMissle( misslePosition, missleVelocity );
    }
       
} // end namespace PatternSpace
