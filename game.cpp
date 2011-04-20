
#include "game.h"

namespace PatternSpace {
    
    void Game::play() {
        repo.load();
        GameStateEnum state = INTRO;
        while (state != EXIT) {
            pGameState = newGameState(state);
            state = pGameState->run(repo);
        }
    }

    std::auto_ptr<GameState> Game::newGameState(GameState::GameStates s) {
        switch(s) {
            case INTRO: {
                std::auto_ptr<GameState> gs(new IntroGameState(repo) );
                return gs;
            }
            case MAIN: {
                std::auto_ptr<GameState> gs(new MainGameState(repo) );
                return gs;
            }
            default:
                throw(0);
            }
    }

        void Repository::load() { /* stub */ }

        // stub
        boost::shared_ptr<Solid> Repository::newSolid(SolidEnum type) {
            switch(type) {
                case ROCK: {
                 	BitmapImage rockImage("images/rock.bmp");
                 	NewtonianMass * pm = new NewtonianMass(1000,2000,20,Vector2d(120,120),Vector2d(0,.1),0.,.1);
                    boost::shared_ptr<Solid> rock ( new SimpleSolid( pm, new BitmapImage(rockImage) ) );
                    return rock;
                }
                case BIG_ROCK: {
                 	BitmapImage rockImage("images/rock.bmp");
                 	NewtonianMass * pm = new NewtonianMass(5000,10000,35,Vector2d(720,320),Vector2d(0,-.1),0.,.03);
                    boost::shared_ptr<Solid> rock ( new SimpleSolid( pm, new BitmapImage(rockImage) ) );
                    return rock;
                }   
                case ALIEN: {
                    boost::shared_ptr<Image> pa1( new BitmapImage("images/alien1-1.bmp") );
                    boost::shared_ptr<Image> pa2( new BitmapImage("images/alien1-2.bmp") );
                    boost::shared_ptr<Image> pa3( new BitmapImage("images/alien1-3.bmp") );
                    AnimatedImage * pa( new AnimatedImage(pa1,5) );
                    pa->add(pa2);
                    pa->add(pa3);
                 	NewtonianMass * pm = new NewtonianMass(100,2000,12,Vector2d(500,300), Vector2d(-.1,0),0,0) ;
                    boost::shared_ptr<Mass> pBaseAlienMass( pm );
                    Mass * alienMass = new DamageMass(pBaseAlienMass, 1000);
                    boost::shared_ptr<Solid> alien(new SimpleSolid( alienMass, pa) );   
                    return alien;
                }
                default:
                    throw(0);
            }
        }
        
//        boost::shared_ptr<Ship> Repository::newShip(ShipEnum type);
//        boost::shared_ptr<Background> Repository::newBackground(BackgroundEnum type) {
            


} // end namespace PatternSpace
