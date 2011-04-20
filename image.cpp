/*
  Implementations for Surface, Image, and Screen
 
  BitmpaImage is basically a shared_ptr<Surface>, but it's at the right level 
of abstraction to cache a set of rotated surfaces.  Plus it implements the
Image interface.

AnimatedImage also implements the Image interface, cycling through it's
images regularly.  It's Decorator-ish, in that it delegates to Image and 
implements the Image interface at the same time.  The difference is it 
delegates to LIST of Images, instead of decorating just one.

I'll probably add an AgregateImage class that also maintains a list of Images,
but displays them at the same time, with offsets, to build complex images.
Of course, AgreateImage could hold AnimatedImages and vice versa; that would
be handy.

*/

#include "image.h"
#include <SDL/SDL_rotozoom.h>		// SDL_gfx Rotozoom

namespace PatternSpace {

/*********************  Surface  *********************/
    
     // Note: the default constructor is protected, since it
     // should only be used for derived classes.
    Surface::Surface():
        surface(0), count(0)
    {}
    
    Surface::Surface( const char* filename) 
    {
        SDL_Surface* rawSurface = SDL_LoadBMP(filename);
        if ( rawSurface ) {
            surface = SDL_DisplayFormat(rawSurface);
            SDL_FreeSurface(rawSurface);
        }
        else {
            surface = 0;
        }
        count = 0;
    }
    Surface::Surface( SDL_Surface * sdlSurface) 
    {
        surface = sdlSurface;
        count = 0;
    }
    
    Surface::~Surface() {
        if (surface) {
            SDL_FreeSurface(surface);
        }
    }

    void Surface::blit(Surface& onto, Vector2d location) 
    {
        SDL_Rect rectLocation;  // where on the screen to draw, in SDL langauge
        rectLocation.x = int(location.x());
        rectLocation.y = int(location.y());
        // TODO: this needs to be moved to Image
        Uint32 transparent = SDL_MapRGB(onto.surface->format,0,0,0);
        SDL_SetColorKey(surface, SDL_SRCCOLORKEY, transparent);       
        SDL_BlitSurface(surface, 0, onto.surface, &rectLocation);
    }

    Surface* Surface::rotatedBy(double angle) 
    {
        return new Surface( rotozoomSurface(surface, -angle, 1, 1) );
    }
    
    Vector2d Surface::size() {
        return Vector2d(surface->w,surface->h);
    }

/*********************  Image  *********************/

    BitmapImage::BitmapImage(const char * filename) 
    {
        surface = new Surface(filename);
        surface->count++;
    }
    BitmapImage::BitmapImage(const BitmapImage& other) 
    {
        surface = other.surface;
        surface->count++;
    }
    BitmapImage::~BitmapImage() 
    {
        surface->count--;
        if (surface->count == 0) delete surface;
    }
    
    BitmapImage& BitmapImage::operator=(const BitmapImage& other) 
    {
        other.surface->count++;
        surface->count--;
        if ( surface->count == 0 ) delete surface;
        surface = other.surface;
        return *this;
    }
    
    //  Note: in the case of assignment to self, surface and other.surface
    //  point to the same Surface.  So, we increment then decrement the
    //  count, leaving it unchanged.  The count won't be zero, so we don't
    //  free the object.  Finally, copying other.surface to surface doesn't
    //  change anything, because they already were the same.
    void BitmapImage::draw(Surface& screen, Vector2d location, double angle) 
    {
        if (angle == 0) {
            surface->blit(screen, location - ( surface->size()/2 ) );
        } else {

            std::auto_ptr<Surface> rotatedSurface( surface->rotatedBy(angle) );
            rotatedSurface->blit(screen,location - (rotatedSurface->size()/2) );
        }
    }
    
    Vector2d BitmapImage::size() 
    {
        if ( surface ) {
            return surface->size();
        } else {
            return Vector2d(0,0);
        }
    }

/*********************  AnimatedImage  *********************/

    AnimatedImage::AnimatedImage(boost::shared_ptr<Image> pFirstImage, int rate):
        rate(rate), count(0) 
    {
        if (pFirstImage.get()) {
            images.push_back(pFirstImage);
        }
        ppImage = images.begin();
    }
    
    AnimatedImage& AnimatedImage::add(boost::shared_ptr<Image> pImage) 
    {
        if (pImage.get() == 0) throw(0);
        images.push_back(pImage);
        ppImage = images.begin();
        count = 0;
        return *this;
    }
    void AnimatedImage::draw(Surface& screen, Vector2d at, double angle) 
    {
        if (++count == rate) {
            count = 0;
            ppImage++;
            if ( ppImage == images.end() ) {
                ppImage = images.begin();
            }
        }
        (*ppImage)->draw(screen, at, angle);
    }


/*********************  Screen  *********************/
// Note: the exits aren't really appropriate and should be moved up.

    Screen::Screen():
        _origin(), height(600), width(800) 
    {
        
        if(SDL_Init(SDL_INIT_EVERYTHING) == -1){
            fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
            exit(1);
        }
        // 32 is color depth
        //surface = SDL_SetVideoMode(width, height, 32, SDL_FULLSCREEN|SDL_HWSURFACE|SDL_DOUBLEBUF);
        surface = SDL_SetVideoMode(width,height, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
        
    	if(surface == NULL){
	       fprintf(stderr, "Unable to set video mode: %s\n", SDL_GetError());
    		exit(1);
    	}
    	SDL_ShowCursor(SDL_DISABLE);
    }
    
    void Screen::clear() { SDL_FillRect(surface, NULL, 0); }
    
    void Screen::flip() { SDL_Flip(surface); }
    
    Screen::~Screen() 
    {  
        SDL_Quit(); 
        // this is really important!  Screen derives from Surface, and 
        // ~Surface() will attempt to free this SDL surface unless this
        // pointer is 0.
        surface = 0;
                
        }

    Vector2d Screen::size() 
    {
        return Vector2d( double(width), double(height) );
    }
 
    Vector2d Screen::origin() const 
    {
        return _origin;
    }
    
    Screen& Screen::origin(const Vector2d& newOrigin) 
    {
        _origin = newOrigin;
    }
    
/*********************  SimpleSprite  *********************/

    void SimpleSprite::draw(Screen& screen) 
    {
        Vector2d screenPosition = spritePosition() - screen.origin();
        image().draw(screen, screenPosition, spriteAngle() );
    }
  
/*********************  Background  *********************/

    // tiles the screen with the background image.
    void Background::draw(Screen& screen) 
    {
        double screenWidth = screen.size().x();
        double screenHeight = screen.size().y();
        double width = size().x();
        double height = size().y();

        if (width == 0 or height == 0) return;  // avoid infinite loop

        Vector2d origin = screen.origin();
        double xbegin = fmod(-origin.x(), width);
        double ybegin = fmod(-origin.y(), height);
        if ( xbegin > 0 ) xbegin -= width;
        if ( ybegin > 0 ) ybegin -= height;

        for( double y = ybegin; y < screenHeight; y += height ) {
            for( double x = xbegin; x < screenWidth ; x += width) {
                surface->blit(screen, Vector2d(x, y) );
            }
        }
    }
        
} // end namespace PatternSpace

