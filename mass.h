/*
  Mass, NewtonianMass, FrictionMass, LinearMass

  Mass models a two dimensional physical mass. The basic usage is to apply 
forces to the Mass with the hit() and push() methods.  The next time you call 
step(), those forces will be used to update the Mass's velocity, and the 
velocity will be used to update the Mass's position, and likewise for rotation.
Forces do not persist, but must be re-applied each step.

  You might know that this is a pretty poor way to run a physics simulation; for
example, energy will not be conserved!  It would be more accurate to work
with first order differential equations, and/or use a real diff-eq solving 
method (like Runga-Kutta) to advance the simulation.  Using first order diff-eqs
would be a fairly radical departure (but is really the only way to guarantee
the conserved quantities) and would make it harder for me to shove my little 
masses around whenever I wanted to.  Using better integration approximations
would be less of a depature.  But I added friction to the user's "space ship"
just to make it easier to control; accurate physics isn't really a priority.

  Mass is an Abstract Base Class, but provides a some default implementations
by giving definitions for some of the pure virtual functions, per Item 34 of
Effective C++.

  The difference between push/torque and hit/twist is that push/torque is
applied overtime, so is proportional to deltaTime in a step(), while hit/twist
is used for impulses (impacts, collisions) that are instantaneous.

  NewtonianMass is the most basic implementation; it takes all the defaults,
and implements the properties as thin wrappers around private members.  Note 
that the names of the private members make use the common physics notation.

  FrictionMass adds friction to both velocity and rotation.
  
  A LinearMass always points it's Image in the direction it's moving.  I 
intended this to be a trivial way to get missles to look like they're pointing
forward, but it looks weird when the player is moving fast.

  Note: the DamageMass class didn't work out, because I decided damage was a 
Solid level idea, not a Mass level idea.  It is a working, if oversized,
example of the Decorator pattern.

  Note that the interaction functions are not friends, because they don't need
to be; Mass's public interface suffices.  Note also that all interactions will
have the same signature, so that's an abstraction opportunity.

*/

#ifndef PATTERN_SPACE_MASS_INCLUSION_GUARD
#define PATTERN_SPACE_MASS_INCLUSION_GUARD

#include <boost/shared_ptr.hpp>

#include "vector2d.h"
namespace PatternSpace {
    
/*********************  Mass  *********************/
    // ABC
    class Mass {
    public:
        // virtual destructor supports polymorphism
        virtual ~Mass() {}
        
        // Use these to tell a mass how and when to move.
        virtual Mass& push(const Vector2d force) = 0;
        virtual Mass& hit(const Vector2d impulse) = 0;
        // default implementations available for push(f,p) and hit(i,p)
        // they use push/torque and hit/twist.
        virtual Mass& push(const Vector2d force, const Vector2d offset) = 0;
        virtual Mass& hit(const Vector2d impulse, const Vector2d offset) = 0;
        virtual Mass& torque(double torque) = 0;
        virtual Mass& twist(double suddenTorque) = 0;
        virtual void step(double deltaTime) = 0;

        // directly change the possition.  Can be used by step(), or as a
        // backdoor to move the mass to a particular position.
        virtual Mass& translate(Vector2d deltaPosition) = 0;
        
        // access physical properties
        virtual double mass() const  = 0;
        virtual double moment() const = 0;
        // default implementation available for moment()
        virtual Vector2d position() const  = 0;
        virtual Vector2d velocity() const = 0;
        virtual double angle() const = 0;
        virtual double rotation() const = 0; // angle rate of change
        virtual double radius() const = 0;
        
        // access status
        virtual bool isDead() const = 0;
        
        // Note: functions such as energy(), momentum() etc. should
        // be defined in mass and not be virtual because they are invariants,
        // and are mathematically defined in terms of the above properties.
        // I don't need these yet, but this is where they should go when I do.
    };

/*********************  NewtonianMass  *********************/
    class NewtonianMass: public Mass {
    public:
        explicit NewtonianMass(double mass,
                              double moment,
                              double radius,
                              Vector2d position,
                              Vector2d velocity,
                              double angle,
                              double rotation);

        NewtonianMass(Mass&);
        
        // Note: default destuctor, copy, and assignment are fine.

        Mass& push(const Vector2d force);
        Mass& hit(const Vector2d impulse);
        Mass& twist(double suddenTorque);
        Mass& torque(double torque);
        void step(double deltaTime);
        
        // take the defaults for offset push and hit
        Mass& push(const Vector2d force, const Vector2d offset) {
            return Mass::push(force, offset);
        }
        Mass& hit(const Vector2d impulse, const Vector2d offset) {
            return Mass::hit(impulse, offset);
        }

        Mass& translate(Vector2d deltaPosition);
        
        // access physical properties
        double mass() const {return m;}
        double moment() const {return I;}
        Vector2d position() const {return p;}
        Vector2d velocity() const {return v;}
        double angle() const {return a;}
        double rotation() const {return o;}
        double radius() const {return r;}
        
        // access status
        bool isDead() const { return false;}
        bool isDamaged() const {return false;}
        
    protected:
        // Template Pattern: you can re-use the default implementation
        // in step() simply by overriding these two protected methods:
        virtual Vector2d totalForce(double dT) const;
        virtual double totalTorque(double dT) const;
        
    private:
        void reset();  // clears all applied forces
        void clean();  // cleans up angles

        double m;  // mass
        double I;  // moment of inertia
        double a;  // current angle
        double o;  // (omega) angular rate of rotation
        double tsum;  // total torque
        double stsum; // total sudden torque
        double r;  // radius
        Vector2d p;  // position
        Vector2d v;  // velocity
        Vector2d fsum;  // total force
        Vector2d isum;  // total impuse
    };  // end class NewtonianMass

/*********************  FrictionMass  *********************/
    class FrictionMass: public NewtonianMass {
    public:
        FrictionMass(double mass,
                     double moment,
                     double radius,
                     Vector2d position,
                     Vector2d velocity,
                     double angle,
                     double rotation,
                     double velocityFriction,
                     double turnFriction):
            NewtonianMass(mass,moment,radius,position,velocity,angle,rotation),
            velocityFriction(velocityFriction),
            turnFriction(turnFriction) {}
            
    protected:
        Vector2d totalForce(double dT) const {
            Vector2d friction = - velocityFriction * dT * velocity() * mass();
            return NewtonianMass::totalForce(dT) + friction;
        }
        double totalTorque(double dT) const {
            double friction = - turnFriction * dT * rotation() * moment();
            return NewtonianMass::totalTorque(dT) + friction;
        }
    private:
        double velocityFriction;
        double turnFriction;
    }; // end class FrictionMass
    
/*********************  LinearMass  *********************/
    // a LinearMass always points in the direction it's travelling
    class LinearMass: public NewtonianMass {
    public:
        LinearMass(double mass,
                   double moment,
                   double radius,
                   Vector2d position,
                   Vector2d velocity):
            NewtonianMass(mass,moment,radius,position,velocity,0,0) {}
        double angle() const { return velocity().angle(); }
    }; // end class LinearMass
    
    /*********************  Interactions  *********************/
    // applies the gravitational force between two Masses to each.
    void gravitate( Mass& m1, Mass& m2);
    
    // bounce objects off each other in a simple way.
    // this is an elastic collision ignoring tangential friction (no
    // angular momentum transfer.)  Also, the masses are brute force shifted
    // apart so as not to overlap.  This strategy works well for sparse
    // collisions, but will cause serious weirdness with tightly packed
    // solids.
    void collision( Mass& m1, Mass& m2);
    

} // end namespace PatternSpace
#endif // PATTERN_SPACE_MASS_INCLUSION_GUARD
