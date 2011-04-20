/*
    Ship, Controls
    
  The Ship is the Solid that represents the user, so it responds to key presses
by accelerating.
  
  Note that Ship implements the Solid interface.  That means we can simply add
it to the Universe and have it participate in the physics simulation and get
drawn to the screen, no differently than any other Solid.

  Note that the Ship 'has-a' Mass and Image.  This means we can swap the
implementations of the ship's Mass and Image at run-time.  In particular,
we can offer the user a different 'feel' by changing the implementation class
of the Mass; I've found that FrictionMass is the most intuitive to control.

*/
#ifndef PATTERN_SPACE_SHIP_INCLUSION_GUARD
#define PATTERN_SPACE_SHIP_INCLUSION_GUARD

#include <boost/shared_ptr.hpp>

#include "solid.h"

namespace PatternSpace {
    
/*********************  Controls  *********************/
    // interface used to communicate to user input to a Solid
    class Controls {
    public:
        virtual void up(bool) = 0;
        virtual void down(bool) = 0;
        virtual void left(bool) = 0;
        virtual void right(bool) = 0;
        virtual void primary(bool) = 0;
    }; // end class Controls
        
/*********************  Ship  *********************/
    class Ship: public NormalSolid, public Controls {
    public:
        Ship( std::auto_ptr<Mass> pMass, std::auto_ptr<Image> pImage):
            NormalSolid(pMass,pImage,10000,0,3),
            upState(false),
            downState(false),
            leftState(false),
            rightState(false),
            primaryState(false),
            fireMissle(false) {}
        
        void up(bool);
        void down(bool);
        void left(bool);
        void right(bool);
        void primary(bool);
        void step(double deltaTime);
        bool hasSpawn() const;
        boost::shared_ptr<Solid> nextSpawn();
        
    private:
        const static double ENGINE_THRUST;
        const static double ENGINE_REVERSE_THRUST;
        const static double TURN_THRUST;
        
        bool upState;
        bool downState;
        bool leftState;
        bool rightState;
        bool primaryState;
        
        bool fireMissle;        // true while waiting to fire a missle.
    }; // end class Ship
    
} // end namespace PatternSpace
#endif  // PATTERN_SPACE_SHIP_INCLUSION_GUARD
