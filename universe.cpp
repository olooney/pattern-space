/*
  Implementation for Universe
"In theory, the Universe is implemented in LISP, but in practice it's mostly
hacked together with Perl scripts."

  The direct checks on the Solid::descriptor() are ugly and will scale poorly.
That needs to be factored out, perhaps by having a table that defines the
allowed interactions between different types of Solids.  

  One service that Universe could provide (but doesn't currently) is only
running interactions between objects that are fairly close to the Ship.  If
"active" area was small compared to the entire universe, this could result
in considerable savings.

*/
#include "universe.h"
#include "factories.h"

namespace PatternSpace {

/*********************  Universe  *********************/
    Universe::Universe(Screen* iscreen, Background* ibackground):
        screen(*iscreen), background(*ibackground)
    {}
    
    Universe::~Universe() {}
    
    Vector2d Universe::center()
    {
        return screen.origin() + (screen.size() / 2);
    }
    
    Universe& Universe::center(Vector2d newCenter) 
    {
        screen.origin( newCenter - (screen.size() / 2) );
        return *this;
    }
    
    Universe& Universe::add( boost::shared_ptr<Solid> pSolid) 
    {
        addList.push_back(pSolid);
        return *this;
    }
    Universe& Universe::simulateAll(double deltaTime) 
    {
        interactAll();  // n^2 interactions between solids
        normalizeAll(); // clean up the allSolids list
        stepAll(deltaTime);    // advance each solid
    }
    
    // n^2 interactions between solids
    Universe& Universe::interactAll() 
    {
        std::list<boost::shared_ptr<Solid> >::iterator ppSolid1;
        std::list<boost::shared_ptr<Solid> >::iterator ppSolid2;
        for( ppSolid1 = allSolids.begin(); ppSolid1 != allSolids.end(); ppSolid1++) {
            if ( (**ppSolid1).descriptor() != 2) {
                Lock lock1( **ppSolid1 );
                ppSolid2 = ppSolid1;
                ppSolid2++;
                for( ; ppSolid2 != allSolids.end(); ppSolid2++ ) {
                    if ( (**ppSolid2).descriptor() != 2) {
                        Lock lock2( **ppSolid2 );
                        gravitate( **ppSolid1, **ppSolid2);  // TODO: allow the client to register arbitrary interaction functions
                        collision( **ppSolid1, **ppSolid2);
                    }
                }
            }
        }
        return *this;
    }
    
    // predicate used by normalizeAll
    bool isDead(boost::shared_ptr<Solid> pSolid) { return pSolid->isDead(); }
    
    // add newly spawned Solids to the universe, and clean up the dead ones.
    Universe& Universe::normalizeAll() 
    {
        Lock lock(allResource);

        // add explosions where objects died.
        std::list<boost::shared_ptr<Solid> >::iterator ppSolid;    
        for( ppSolid = allSolids.begin(); ppSolid != allSolids.end(); ppSolid++) {
            if ( isDead( *ppSolid) && ((**ppSolid).descriptor() != 2) ) {
                boost::shared_ptr<Solid> pExplosion = newExplosion( (**ppSolid).position(), (**ppSolid).velocity() );
                add( pExplosion );
            }
        }
        
        // ask each Solids if it would like to spawn some new Solids.
        for( ppSolid = allSolids.begin(); ppSolid != allSolids.end(); ppSolid++) {
            while ( (**ppSolid).hasSpawn() ) {
                add( (**ppSolid).nextSpawn() );
            }
        }
        
        allSolids.remove_if( isDead );
        allSolids.splice( allSolids.end(), addList);
    }
    
    // update the velocity and position of each solid according to
    // applied forces.
    Universe& Universe::stepAll(double deltaTime) 
    {
        std::list<boost::shared_ptr<Solid> >::iterator ppSolid;
        for( ppSolid = allSolids.begin(); ppSolid != allSolids.end(); ppSolid++) {
            Lock lock( **ppSolid );
            (*ppSolid)->step(deltaTime);
        }            
        return *this;
    }
    
    // draw each solid.
    Universe& Universe::drawAll() 
    {
        Lock allLock(allResource);
        screen.clear();
        background.draw(screen);
        std::list<boost::shared_ptr<Solid> >::iterator ppSolid;
        for( ppSolid = allSolids.begin(); ppSolid != allSolids.end(); ppSolid++) {
            Lock lock( **ppSolid );
            (*ppSolid)->draw(screen);
        }
   		screen.flip();

        return *this;
    }
    
} // end namespace PatternSpace
