/*
  Solid, NormalSolid

  Solid is a abstract base class the supports the Mass, Sprite, and 
Resource interfaces.  So, it's something that can participate in both physics 
and display.  Those will be handled on different threads, so the Resource is
needed to control access.

  Note that you don't *have* to lock the Solid to access it's members.  Like
the reference count member of the raw resource in a reference counting
implementation, Resource is simply made available, with the understanding that
another class (in this case Universe) is expected to make proper use of it.

  Solid has a hack field, descriptor.  The basic idea here is that because we
can put together a Solid in a very dynamic way that is independent of what kind
of thing it really is.  We need something that says what the object is from
the point of view of the simulation: just a special affect, an inanimate object,
an enemy.  I haven't decided on a proper object oriented design for this, so
simply assigning Solids a numeric code by type and handle it in an ad-hoc way.
I'll be able to refactor to a more scalable design later; for now, it is the
Simplist Thing That Could Possibly Work.  

  A NormalSolid is the most convenient way to implement Solid: it simply
delegates the Mass and Sprite aspects of it's interface to member objects, and
relies on the default implementation provided for Resource.  Because it simply
delegates, it's very simple and is implemented mostly inline.

*/
#ifndef PATTERN_SPACE_SOLID_INCLUSION_GUARD
#define PATTERN_SPACE_SOLID_INCLUSION_GUARD

#include <boost/shared_ptr.hpp>

#include "vector2d.h"
#include "image.h"
#include "mass.h"
#include "lock.h"

namespace PatternSpace {

/*********************  Solid  *********************/
    // ABC
    class Solid: public Mass, public Sprite, public Resource {
    public:
        virtual ~Solid() {}
        virtual bool isDead() const = 0;
        virtual int descriptor() const = 0;
        virtual bool hasSpawn() const { return false;}
        virtual boost::shared_ptr<Solid> nextSpawn() { /* don't call me */ }

    }; // end class Solid 

/*********************  NormalSolid  *********************/
    class NormalSolid: public Solid, public SimpleSprite {
    public:
        NormalSolid(std::auto_ptr<Mass> pMass, std::auto_ptr<Image> pImage,
                    int hitPoints,int lifetime,int descriptor):
            pMass(pMass), pImage(pImage),hitPoints(hitPoints), life(lifetime),  _descriptor(descriptor),
             dead(false), damage(0), age(0.0)
        {}
        // Note: since I've ordered the initialization list to match the
        // parameters, it's worth pointing out that the members will be
        // initialized in the order they're defined in the class, not the order
        // they're listed here.

        bool isDead() const { return dead; }
        int descriptor() const { return _descriptor; }
        bool hasSpawn() const { return false;}
        boost::shared_ptr<Solid> nextSpawn() { /* not ready yet. */ }
        
        // implement the Mass interface by delegating to m
        Mass& push(const Vector2d force) { return pMass->push(force); }
        Mass& hit(const Vector2d impulse);  // additional behavior added in .cpp
        Mass& push(const Vector2d force, const Vector2d offset) { return pMass->push(force, offset);  }            
        Mass& hit(const Vector2d impulse, const Vector2d offset) { return pMass->hit(impulse, offset); }
        Mass& torque(double torque) { return pMass->torque(torque); }
        Mass& twist(double suddenTorque) { return pMass->twist(suddenTorque); }
        void step(double deltaTime);  // additional behavior added in .cpp
        Mass& translate(Vector2d deltaPosition) { return pMass->translate(deltaPosition); }
        double mass() const { return pMass->mass(); }
        double moment() const { return pMass->moment(); }
        Vector2d position() const { return pMass->position(); }
        Vector2d velocity() const { return pMass->velocity(); }
        double angle() const { return pMass->angle(); }
        double rotation() const { return pMass->rotation(); }
        double radius() const { return pMass->radius(); }

    protected:
        int _descriptor;
        bool dead;
        int hitPoints;
        double damage;
        int life;
        double age;
        std::auto_ptr<Mass> pMass;
        std::auto_ptr<Image> pImage;
        void die(); 

        // implement the SimpleSprite virtual functions.  SimpleSprite provides
        // a basic implementation of draw() in terms of these.  (Item 35, the
        // Non-Virtual Interface idiom.)
    private:
        double spriteAngle() const { return angle(); }
        Vector2d spritePosition() const { return position(); }
        Image& image() { return *pImage; }
    public:
        void draw( Screen& screen) { SimpleSprite::draw(screen); }

    };  // end class NormalSolid

} // end namespace PatternSpace
#endif  // PATTERN_SPACE_SOLID_INCLUSION_GUARD
