/*
  Implementation for Mass

  Mostly implements a second-order 2D classical physics simulation in the
naive way.

  Note that the interactions only make use of the Mass interface, and have
no knowledge of the various implementations.
  
*/

#include "mass.h"

namespace PatternSpace {
    
/*********************  Mass  *********************/
    // TODO: use templates to combine these two implementations
    Mass& Mass::push(const Vector2d force,const Vector2d offset)
    {
        push(force);
        if ( offset.magnitude() > .0001 ) {
            torque( cross( force, offset) );
        }
        return *this;
    }
    
    Mass& Mass::hit(const Vector2d impulse,const Vector2d offset)
    {
        hit(impulse);
        if ( offset.magnitude() > .0001 ) {
            twist( cross( impulse, offset ) );
        }
        return *this;
    }
    
    // moment of a sphere.  (useful default implementation.)
    // "I have a solution, but it only works for spherical chickens in
    //  a vaccuum!"
    double Mass::moment() const 
    {
        double r = radius();
        return (2.0/5.0) * mass() * r*r;
    }

/*********************  NewtonianMass  *********************/
    NewtonianMass::NewtonianMass(double mass,
                                 double moment,
                                 double radius,
                                 Vector2d position,
                                 Vector2d velocity,
                                 double angle,
                                 double rotation):
        m(mass),
        I(moment),
        r(radius),
        p(position),
        v(velocity),
        a(angle),
        o(rotation),
        tsum(0),
        stsum(0),
        isum(Vector2d(0,0)),
        fsum(Vector2d(0,0)) 
    {}
            
    NewtonianMass::NewtonianMass(Mass& rhs):
        m(rhs.mass()),
        I(rhs.moment()),
        r(rhs.radius()),
        p(rhs.position()),
        v(rhs.velocity()),
        a(rhs.angle()),
        o(rhs.rotation()),
        tsum(0),
        stsum(0),
        isum(Vector2d(0,0)),
        fsum(Vector2d(0,0))
    {}
            
    Mass& NewtonianMass::translate(const Vector2d deltaPosition) {
        p += deltaPosition;
        return *this;
    }
    
    Mass& NewtonianMass::push(const Vector2d force) {
        fsum += force;
        return *this;
    }
    Mass& NewtonianMass::hit(const Vector2d impulse) {
        isum += impulse;
        return *this;
    }   
    Mass& NewtonianMass::torque(double torque) {
        tsum += torque;
        return *this;
    }
    Mass& NewtonianMass::twist(double suddenTorque) {
        stsum += suddenTorque;
        return *this;
    }
    void NewtonianMass::reset() {
        Vector2d zero(0,0);
        fsum = zero;
        isum = zero;
        tsum = 0;
        stsum = 0;
    }
    void NewtonianMass::clean() {
        a = fmod(a,360);
        o = fmod(o,360);
    }
    Vector2d NewtonianMass::totalForce(double deltaTime) const {
        return (fsum * deltaTime) + isum;
    }
    double NewtonianMass::totalTorque(double deltaTime) const {
        return (tsum * deltaTime) + stsum;
    }
    void NewtonianMass::step(double deltaTime) {
        v +=  ( totalForce(deltaTime) / mass() );
        p +=  v;
        o += ( totalTorque(deltaTime) / moment() );
        a += o;
        reset();
        clean();   
    }
    
/*********************  Interactions  *********************/

    // applies the gravitational force between two Masses to each.
    // Effective C++ item 23: prefer non-member non-friends.
    // G := gravitation constant
    // R := relative position vector
    // r := distance between the masses
    // f := magnitude of the graviational force
    // F := Force vector on mass1.
    void gravitate( Mass& mass1, Mass& mass2) {
        static const double G = .001;
        Vector2d R = mass2.position() - mass1.position();
        double r = R.magnitude();
        
        // gameplay kludge.  limit forces very near large masses.
        double threshold = mass1.radius() > mass2.radius() ?
                            3 * mass1.radius() :
                            3 * mass2.radius();
        if (r < threshold) r = threshold;
        
        double f = G * mass1.mass() * mass2.mass() / (r*r);
        Vector2d F = f * R.unit();
        mass1.push( F );
        mass2.push( -F );
    }
    
    void collision( Mass& mass1, Mass& mass2 ) {
        Vector2d R = mass2.position() - mass1.position();
        double r = R.magnitude();
        double overlap = ( mass1.radius() + mass2.radius() ) - r;
        if ( overlap < 0 ) return;
        Vector2d axis = R.unit();
        
        // calculate the velocity of the center of mass along the axis.
        double p1 = dot( mass1.velocity(), axis) * mass1.mass();
        double p2 = dot( mass2.velocity(), axis) * mass2.mass();
        double vc = ( p1 + p2) /( mass1.mass() + mass2.mass() );
        
        // applying this impulse will reverse the mass1 relative to the center
        // of mass, i.e. an elastic collision.
        double impulse = 2*( p1 - vc * mass1.mass());
        // Impulse Vector
        Vector2d I = axis * impulse;

        // ok, we're ready to do the actual collision.  Before we do, we
        // move the two masses so they don't overlap.  The +1 adds one pixel
        // of padding.
        mass1.translate( -axis * (overlap+1));
        mass2.translate( axis * (overlap+1) );
        // smack!
        mass1.hit(- I);
        mass2.hit(I);
    }

    
} // end namespace PatternSpace
