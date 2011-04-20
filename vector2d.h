/*
  class Vector2d

  Pattern Space is 2d, so we need a good 2d vector for forces, positions,
velocities, and accelerations.  We use doubles even though the basic scale is
pixels; this gives us smoother animation and less rounding error.

  While it would be possible to use the C++ typesystem to enforce the units
of the vectors, I don't believe there's significant room for error in this
case.  

  Vector2d is entirely inline for "performance reasons," but I haven't actually
measured the impact.

Instead of get/set for access/mutators, I prefer to simply use the attribute
name and let the signature distinguish the two: if it takes a parameter, it's
a mutator, if not, it's a access method.

Vector2d in a POD, and relies on the default value semantics.

  Note: Vector2d doesn't do any bounds checking.  In fact, nothing does.  I
suppose if you fly the ship far enough, it will eventually wrap or crash or
something.  This has no effect today, but could be a problem someday. 

*/
#ifndef PATTERN_SPACE_VECTOR_INCLUSION_GUARD
#define PATTERN_SPACE_VECTOR_INCLUSION_GUARD

#include<math.h>

namespace PatternSpace {
    
/*********************  Vector2d  *********************/
    class Vector2d {
        public:
            Vector2d(): _x(0), _y(0) {}
            // TODO: it's not obvious that the single argument constructor
            //  will result in a unit vector at the given angle.
            Vector2d(double angle) 
            {
                // convert from degrees to radians.
                double radians = 0.0174532925 * angle;
                // Note: angle 0 is straight up in SDL coords; that's (0,-1).
                _x = sin(radians);
                _y = - cos(radians);
            }
            Vector2d(double i_x, double i_y): _x(i_x), _y(i_y) {}
            // default assignment, copy, and destructors are fine.
            
            const double x();
            const double y();
            Vector2d& x(double);
            Vector2d& y(double);
            Vector2d& clear() { x(0); y(0); return *this; };
    
            double magnitude() const;
            const double angle();
            const Vector2d unit();  // returns a unit vector or (0,0)
            const Vector2d rotatedBy(double angle);
     
            // using operator* would be unclear.
            inline friend double dot(const Vector2d& left, const Vector2d& right)
            {
                return left._x * right._x + left._y * right._y;
            };
            // Note: because both vectors are in the xy plane, the cross product
            // vector will along the z-axis.  This function actually returns
            // the z component of the cross product. (right-handed coordinates)
            inline friend double cross(const Vector2d& left, const Vector2d& right) 
            {
                return (left._x * left._y) - ( left._y * right._x);
            }
                
    
            Vector2d friend operator+(const Vector2d&, const Vector2d& );
            Vector2d friend operator-(const Vector2d&, const Vector2d& );
            Vector2d& operator+=(const Vector2d&);
            
            Vector2d friend operator*(const Vector2d&, double);
            Vector2d friend operator/(const Vector2d&, double);
            Vector2d friend operator*(double scale, const Vector2d& vector);
            Vector2d& operator*=(double);
            Vector2d operator-();
            
            // default == and != are fine.
             
        private:
            double _x;
            double _y;
    };
    
    const inline double Vector2d::x() { return _x; }
    const inline double Vector2d::y() { return _y; }

    inline Vector2d& Vector2d::x(double n_x) {
        _x = n_x;
        return *this;
    }

    inline Vector2d& Vector2d::y(double n_y) {
        _y = n_y;
        return *this;
    }
    
    inline double Vector2d::magnitude() const  {
        return sqrt( _x*_x + _y*_y );
    }
    
    // returns a unit vector in the same direction or (0,0).
    const inline Vector2d Vector2d::unit() {
        double mag = magnitude();
        Vector2d ret;
        if ( mag > 0 ) {
            ret._x = _x/mag;
            ret._y = _y/mag;
        }
        return ret;
    }
    
    const inline Vector2d Vector2d::rotatedBy(double angle)
    {   
        // convert degrees to radians
        double radians = 0.0174532925 * angle;
        return Vector2d(
            _x * sin(radians) + _y * cos( radians),
            _y * sin(radians) - _x * cos( radians) );
    }
    
    const inline double Vector2d::angle() 
    {
        // avoid degenerate case
        if ( _x == 0 ) {
            if ( _y <= 0 ) return 0;
            else return 180;
        }
        double firstAndThirdQuadrantAngle = - atan(_x/_y);
        if (_y <= 0 ) return firstAndThirdQuadrantAngle * 57.2957795 ;
        else return 180 + 57.2957795 * firstAndThirdQuadrantAngle;
    }
        
    
    inline Vector2d operator+(const Vector2d& left, const Vector2d& right) 
    {
        Vector2d ret;
        ret._x = left._x + right._x;
        ret._y = left._y + right._y;
        return ret;
    }
    
    inline Vector2d operator-(const Vector2d& left, const Vector2d& right) 
    {
        Vector2d ret;
        ret._x = left._x - right._x;
        ret._y = left._y - right._y;
        return ret;
    }
    
    inline Vector2d& Vector2d::operator+=(const Vector2d& right) 
    {
        _x += right._x;
        _y += right._y;
        return *this;
    }
    
    inline Vector2d operator*(const Vector2d& vector, double scale) 
    {
        Vector2d ret;
        ret._x = vector._x * scale;
        ret._y = vector._y * scale;
        return ret;
    }
    inline Vector2d operator*(double scale, const Vector2d& vector) 
    {
        return (vector * scale);
    }
    
    inline Vector2d operator/(const Vector2d& vector, double scale) 
    {
        return vector * (1.0 /scale);
    }

    
    inline Vector2d& Vector2d::operator*=(double scale) 
    {
        _x *= scale;
        _y *= scale;
        return *this;
    }
    inline Vector2d Vector2d::operator-() 
    {
        Vector2d ret;
        ret._x = - _x;
        ret._y = - _y;
        return ret;
    }

} // end namespace PatternSpace

#endif // PATTERN_SPACE_VECTOR_INCLUSION_GUARD
