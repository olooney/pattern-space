/*
   Surface, Image, Screen, and Sprite
   
   Surface wraps an SDL_Surface and also contains a reference count.
   
   Image contains a Surface, and provides value semantics to images,
using reference counting to share Surfaces.
   
   Screen is a subclass of Surface that mostly handles the initialization
and cleanup of an SDL context.

  Sprite is the Abstract Base Class for visible objects in PatternSpace.
-------------------------------------------------------------------------
Usage:
  Instantiate a Screen before using any SDL functionality.  This sets up
all the global context behind the scenes.  It is an error to instantiate
more than one Screen at a time.

  clear() the Screen before drawing each frame, and flip() it when you're
done drawing.

  Don't use Surfaces directly; instead, create an Image by loading from
a BMP file.

  Pass the Screen in as the first parameter to draw() to the screen. (You
can draw on any Surface, though.)  Passing an angle argument to draw()
will draw the image rotated clockwise by that angle.  

  The current implementation of Image is performance naive, but Image is at the
correct  level of abstraction to implement a caching scheme for rotated
images.

SimpleSprite is basically a Mixin: by deriving from SimpleSprite and 
implementing the virtual methods, you can easily be a Sprite.

Todo: Surface exposes data right now because I wasn't sure what the interface
should be.  Certainly I don't want to wrap every SDL_ call that takes a 
SDL_Surface.  Surface is only used within the image module, so it 
shouldn't be hard to do later.

*/
#ifndef PATTERN_SPACE_IMAGE_INCLUSION_GUARD
#define PATTERN_SPACE_IMAGE_INCLUSION_GUARD

#include <boost/shared_ptr.hpp>

#include "vector2d.h"
#include <SDL/SDL.h>
#include <stdlib.h>
#include <memory>
#include <vector>

namespace PatternSpace {

/*********************  Surface  *********************/
    class Surface {
    public:
        SDL_Surface * surface;
        int count;

        Surface(const char* filename);
        Surface(SDL_Surface *);
        virtual ~Surface();
        void blit(Surface& onto, Vector2d location);
        Surface* rotatedBy(double angle);
        Vector2d size();
        
    protected:
        // Only a subclass should be creating a Surface without
        // explicitly initializing it somehow.
        Surface();  
    }; // end Surface

/*********************  Image  *********************/    
    class Image {
    public:
        virtual ~Image() {}
        virtual void draw(Surface& screen, Vector2d location, double angle) = 0;
    }; // end class Image
    
    class BitmapImage: public Image {
    public:    
        BitmapImage(const char* filename);
        BitmapImage(const BitmapImage&);
        ~BitmapImage();
        BitmapImage& operator=(const BitmapImage& other);
        
        void draw(Surface& screen, Vector2d at, double angle);
        Vector2d size();
        
    protected:
        Surface *surface;
        
    }; // end BitmapImage

/*********************  AnimatedImage  *********************/
    class AnimatedImage: public Image {
    public:
        AnimatedImage(boost::shared_ptr<Image> pFirstImage, int rate);
        ~AnimatedImage() {}
        AnimatedImage& add(boost::shared_ptr<Image> pImage);
        void draw(Surface& screen, Vector2d location, double angle);

   protected:
        std::vector<boost::shared_ptr<Image> > images;
        std::vector<boost::shared_ptr<Image> >::iterator ppImage;
        int count;
        int rate;

    }; // end class AnimatedImage

/*********************  Screen  *********************/    
    class Screen : public Surface {
    public:
        Vector2d origin() const;
        Screen& origin(const Vector2d& newOrigin);
        Screen();
        ~Screen();
        void clear();
        void flip();
        Vector2d size();

    private:
        int height;
        int width;
        Vector2d _origin;        
    };  // end class Screen
    
    // Sprite is an ABC
    class Sprite {
    public:
        virtual void draw(Screen&) = 0;
        virtual ~Sprite() {}
    }; // end class Sprite
    
    // SimpleSprite uses the Non-Virtual Interface idiom.  The derived class
    // provides the pure virtual functions, and draw() works in terms of those.
    class SimpleSprite: public Sprite {
    protected:
        virtual double spriteAngle() const = 0;
        virtual Vector2d spritePosition() const = 0;
        virtual Image& image() = 0;
    public:
        void draw(Screen& );
    }; // end class SimpleSprite

    class Background: public BitmapImage {
    public:
        Background(const char* filename): BitmapImage(filename) {}
        Background(const BitmapImage& img): BitmapImage(img) {}
        void draw(Screen&);
    }; // end class Background
    
} // end namespace PatternSpace

#endif // PATTERN_SPACE_IMAGE_INCLUSION_GUARD
