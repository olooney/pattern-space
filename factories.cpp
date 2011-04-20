/* 
  Factory functions.  To build a Solid, you need to instantiate at least
one Mass and at least one Image.  You have to make several choices about 
which implementation class to use, and you have to provide a whole slew of
parameters, so it's best to encapsulate those decisions here.  This is the
weakest module in the program, but hey, at least it's encapsulated!

  This module needs the support of background loading (seperate thread), a 
resource control class, and a meta-data file format for defining new Solid
classes.  In fact, it needs a complete overhaul.  

*/

#include <stdio.h>
#include <stdlib.h>
#include "factories.h"

namespace PatternSpace {
    
    boost::shared_ptr<Solid> newRock(Vector2d initialPosition, Vector2d initialVelocity) 
    {        
        std::auto_ptr<Mass> pMass( new NewtonianMass(1000,2000,20,initialPosition,initialVelocity,0.,.1) );
        std::auto_ptr<Image> pImage( new BitmapImage("images/rock.bmp") );
        
        boost::shared_ptr<Solid> pRock ( new NormalSolid(pMass, pImage, 5000, 0, 0 ) );
        return pRock;
    }
    
    boost::shared_ptr<Solid> newBigRock(Vector2d initialPosition, Vector2d initialVelocity) 
    {        
        std::auto_ptr<Mass> pMass( new NewtonianMass(5000,10000,35,initialPosition,initialVelocity,0.,.1) );
        std::auto_ptr<Image> pImage( new BitmapImage("images/big-rock.bmp") );
        
        boost::shared_ptr<Solid> pRock ( new NormalSolid(pMass, pImage, 15000, 0, 0 ) );
        return pRock;
    }
    
    boost::shared_ptr<Solid> newAlien(Vector2d initialPosition, Vector2d initialVelocity) 
    {
        boost::shared_ptr<Image> pa1( new BitmapImage("images/alien1-1.bmp") );
        boost::shared_ptr<Image> pa2( new BitmapImage("images/alien1-2.bmp") );
        boost::shared_ptr<Image> pa3( new BitmapImage("images/alien1-3.bmp") );
        std::auto_ptr<AnimatedImage> paa( new AnimatedImage(pa1,5) );
        paa->add(pa2);
        paa->add(pa3);
        std::auto_ptr<Image> paa2(paa);
        Vector2d p(500,300);
        Vector2d v(0,.1);
        std::auto_ptr<Mass> pAlienMass( new NewtonianMass(100,200,12,initialPosition,initialVelocity,0,0) );
        boost::shared_ptr<Solid> pAlien(new NormalSolid( pAlienMass, paa2, 200, 0, 1) );
        return pAlien;
    }
    
    boost::shared_ptr<Solid> newExplosion(Vector2d initialPosition, Vector2d initialVelocity) 
    {
        boost::shared_ptr<Image> pi1( new BitmapImage("images/explode1.bmp") );
        boost::shared_ptr<Image> pi2( new BitmapImage("images/explode2.bmp") );
        boost::shared_ptr<Image> pi3( new BitmapImage("images/explode3.bmp") );
        boost::shared_ptr<Image> pi4( new BitmapImage("images/explode4.bmp") );
        boost::shared_ptr<Image> pi5( new BitmapImage("images/explode5.bmp") );
        boost::shared_ptr<Image> pi6( new BitmapImage("images/explode6.bmp") );
        boost::shared_ptr<Image> pi7( new BitmapImage("images/explode7.bmp") );
        
        std::auto_ptr<AnimatedImage> pai( new AnimatedImage(pi1,2) );
        pai->add(pi2)
            .add(pi3)
            .add(pi4)
            .add(pi5)
            .add(pi6)
            .add(pi7);
        std::auto_ptr<Image> pai2(pai);
        std::auto_ptr<Mass> pMass( new NewtonianMass(100,200,10,initialPosition,initialVelocity,0.,.1) );
        boost::shared_ptr<Solid> pExplosion(new NormalSolid( pMass, pai2, 100, 50, 2) );
        return pExplosion;
    }

    boost::shared_ptr<Ship> newShip(Vector2d initialPosition, Vector2d initialVelocity)
    {
        BitmapImage shipImage("images/ship.bmp");    
        std::auto_ptr<Mass> pShipMass( new FrictionMass(100,2000,12,initialPosition,initialVelocity,0,0,.0002,.002) );
        std::auto_ptr<Image> pShipImage( new BitmapImage(shipImage) );
        boost::shared_ptr<Ship> pShip( new Ship(pShipMass, pShipImage) );
        return pShip;
    }
    
    boost::shared_ptr<Solid> newMissle(Vector2d initialPosition, Vector2d initialVelocity)
    {
        BitmapImage missleImage("images/missle1.bmp");    
        std::auto_ptr<Mass> pMass( new LinearMass(30,60,5,initialPosition,initialVelocity) );
        std::auto_ptr<Image> pImage( new BitmapImage(missleImage) );
        boost::shared_ptr<Solid> pMissle( new NormalSolid(pMass, pImage,2,500,4 ) );
        return pMissle;
    }

} // end namespace PatternSpace
