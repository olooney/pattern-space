/*
    PatternSpace

  PatternSpace is a C++ playground.  Reading Head First Design Patterns, I was
struck by how silly the examples were.  I decided that it wasn't possible to
explore the concepts of good design in a vaccuum like that.  I thought of
two alternatives: build a playground, or work with a real data set.

By a playground, I mean a program that supplies a lot of objects to interact
with and takes care of a lot of IO issues.  Working with a data set is more
suited to the study of algorithms, while a playground is more suited playing
with object oriented programming.

  I was reading Scott Meyer's Effective C++ while working on this.  Most of
the design decisions were informed in some way by that book.

  I wanted something graphical, but 2D for simplicity.  I knew that physics
could be a rich source of inspiration.  Eventually, I decided on some kind of
2D space game, where you fly around in a little ship using asteroid-like
controls.  PatternSpace is first and foremost a playground in which
I can embed objects and implement various Design Patterns and immediately
watch them do something cool.

  PatternSpace is built on the SDL and SDL_gfx libraries.  This gives me a
simple API to work with that I can take to any machine I choose.  These are
C libraries, so I encapsulate the behavor I want in my own classes.  It also
depends on boost/shared_ptr<>, which is too useful to do without.

  Remember to link in SDL and SDL_gfx when compiling PatternSpace.  Thanks to
Rob Loach, who provided the Dev-C++ dev kit example which got me started.

*/

#include <stdio.h>
#include <stdlib.h>
#include "vector2d.h"
#include "solid.h"
#include "universe.h"
#include "factories.h"
#include "ship.h"

#include <SDL/SDL_framerate.h>		// SDL_gfx Framerate Manager
#include <SDL/SDL_thread.h>
using namespace PatternSpace;

void sendEventsToControls(Controls &c);

bool isRunning = true;

int paint(void *);

int main(int argc, char *argv[]){

    // Instantiate the framework
    Screen screen;
    screen.origin(Vector2d(0,0));
    Background background("images/stars.bmp");
    Universe universe( &screen, &background );

    // Load some stuff
    universe.add( newRock(Vector2d(-400,100), Vector2d(-.2,.1) ) );
    universe.add( newRock(Vector2d(0,500), Vector2d(.05,0) ) );
    universe.add( newRock(Vector2d(250,40), Vector2d(-.3,-.2) ) );
    universe.add( newBigRock(Vector2d(250,10), Vector2d(0,.3) ) );
    universe.add( newRock(Vector2d(-300,-100), Vector2d(.02,-.02) ) );
    universe.add( newRock(Vector2d(-50,-200), Vector2d(.2,-.05) ) );
    universe.add( newAlien(Vector2d(100,150), Vector2d(-.3,0) ) );

    // Load the ship
    boost::shared_ptr<Ship> pShip = newShip( Vector2d(0,0), Vector2d() );
    boost::shared_ptr<Solid> pShipSolid = pShip;
    universe.add( pShipSolid );

    // spawn off graphics thread.
    SDL_Thread * paintThread = SDL_CreateThread( paint, &universe);

    // main thread becomes the physics simulation thread.
    FPSmanager fpsm;
    SDL_initFramerate(&fpsm);
	SDL_setFramerate(&fpsm, 150); // Steps Per Second

	int tick,lastTick;
    double deltaTick;
	tick = SDL_GetTicks();

	while(isRunning){
	    lastTick = tick;
	    tick = SDL_GetTicks();
	    deltaTick = double(tick - lastTick);
	    deltaTick = deltaTick > 1 ? deltaTick : 1;
        universe.simulateAll(deltaTick);
        universe.center( pShip->position() );
        SDL_framerateDelay(&fpsm);
        //SDL_Delay(1);
        sendEventsToControls(*pShip);

	}  // end infinite loop

    // join threads before exiting
    SDL_WaitThread(paintThread, 0);

	return 0;
}  // end main

// this function is meant to be launched as a new thread.  It returns
// only when the global isRunning becomes false.  It paints each frame on
// the screen.
int paint( void * pUniverse) {
    Universe& universe = *static_cast<Universe*>(pUniverse);

    FPSmanager fpsm;
	SDL_initFramerate(&fpsm);
	SDL_setFramerate(&fpsm, 24); // Frames Per Second

    while (isRunning) {
        universe.drawAll();
        SDL_framerateDelay(&fpsm);
    }
}

// map SDL keyboard events to notifications
// ESC key -> quit
// arrow keys ->  notify Controls object
// space bar -> notify Controls object of Primary Action (fire a missle.)
void sendEventsToControls(Controls &controls) {
    SDL_Event event;
	while(SDL_PollEvent(&event)){
        bool state = false;
        switch(event.type) {
            case SDL_QUIT:
    			isRunning = false;
	       		break;
	       	case SDL_KEYDOWN:
                state = true;
                // fall through
            case SDL_KEYUP:
                switch(event.key.keysym.sym) {
                    case SDLK_ESCAPE:
        				isRunning = false;
        				break;
        			case SDLK_UP:
                        controls.up(state);
                        break;
        			case SDLK_DOWN:
                        controls.down(state);
                        break;
        			case SDLK_LEFT:
                        controls.left(state);
                        break;
        			case SDLK_RIGHT:
                        controls.right(state);
                        break;
        			case SDLK_SPACE:
                        controls.primary(state);
                        break;
                }  // end key symbol switch
			break;
		} // end event type switch
    }  // end PollEvent loop
}
