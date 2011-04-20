/*
  Game, GameState, Repository
  
  Game has two responsibilities:  loading data and switching between GameStates.

*/
#ifndef PATTERN_GAME_INCLUSION_GUARD
#define PATTERN_GAME_INCLUSION_GUARD

#include <memory>
#include <boost/shared_ptr.hpp>

#include "solid.h"

namespace PatternSpace {
    
    // Abstract Base Class for game state classes
    class GameState {
    public:
        typedef enum { EXIT, INTRO, MAIN } GameStates;
        virtual ~GameState() {}
        virtual GameStates run(Repository& repo);
    };
    
    // Repository loads data from files and provides factory methods
    // for all the game objects.
    class Repository {
    public:
        // stub.  load should load all the images from bitmaps.
        void load();
        enum SolidEnum { ROCK, BIG_ROCK, ALIEN };
        enum ShipEnum { BASIC };               
        enum BackgroundEnum { STARS }; 
        boost::shared_ptr<Solid> newSolid(SolidEnum type);
        //boost::shared_ptr<Ship> newShip(ShipEnum type);
        //boost::shared_ptr<Background> newBackground(BackgroundEnum type);
    }; // end class Repository
    
    class Game {
    public:
        void play();
    private:
        std::auto_ptr<GameState> pGameState;
        Repository repo;
        std::auto_ptr<GameState> newGameState(GameState::GameStates s);

    }; // end class Game
    
    // Abstract Base Class for game state classes
    class GameState {
    public:
        virtual ~GameState() {}
        virtual Game::GameStateEnum run(Repository& repo);
    };
    
    // TODO: expand this stub
    class IntroGameState: public GameState {
    public:
        IntroGameState(Repository& repo){}
        GameStates run(Repository& repo) {return Game::MAIN;}
    }; // end class IntroGameState

    // TODO: expand this stub    
    class MainGameState: public GameState {
    public:
        MainGameState(Repository& repo) {}
        GameStates run(Repository& repo) {return Game::EXIT; }
    }; // end class MainGameState
    
    
} // end namespace PatternSpace

#endif
