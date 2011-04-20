/*
  Universe class
  
  A Universe is basically a collection of Solids.  Every Solid that's actively
participating in the program will belong to the Universe, and the Universe is
responsible for updating and displaying each object.  The Universe's primary
responsibility is to provide thread safety when using a Solid.

  The primary list of Solids is the allSolids vector.  However, we want to
avoid changing the list while we are iterating over it, any new Solids that are
spawned are not added directly to allSolids, but instead to the addList.  At
the end of each step, we append the allList to the allSolids list.  Note that
before we mutate the allSolids list, we must remember to obtain the allResource.

*/
#ifndef PATTERN_SPACE_UNIVERSE_INCLUSION_GUARD
#define PATTERN_SPACE_UNIVERSE_INCLUSION_GUARD

#include <list>
#include <boost/shared_ptr.hpp>

#include "vector2d.h"
#include "solid.h"
#include "image.h"

namespace PatternSpace {
    
/*********************  Universe  *********************/
    class Universe {
    public:
        Universe(Screen* screen,Background* background);
        ~Universe();
        // bind screen.orgin + (WIDTH/2, HEIGHT/2)
        Universe& center(Vector2d center);
        Vector2d center();
        
        Universe& add( boost::shared_ptr<Solid> );

        // Physics simulation
        Universe& simulateAll(double deltaTime);

        // display
        Universe& drawAll();
    private:
        // Physics simulation
        Universe& stepAll(double deltaTime);     
        Universe& normalizeAll();   
        Universe& interactAll();
        
        std::list< boost::shared_ptr<Solid> > addList;
        std::list< boost::shared_ptr<Solid> > allSolids;
        Resource allResource;  // lockable resource for the all list
        Screen& screen;
        Background& background;
        
        // prevent copying or assignment
        Universe& operator=(Universe&);
        Universe(Universe&);

    }; // end class Universe
        
} // end namespace PatternSpace
#endif  // PATTERN_SPACE_UNIVERSE_INCLUSION_GUARD
