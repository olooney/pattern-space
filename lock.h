/*
    Resource, Lock
    
  Wraps SDL thread syncronization fuctionality in C++ objects.
  
  Resource is a thin wrapper around SDL_Semaphore.

  Lock implements the RAII idiom: simply instantiate a Lock on a Resource at
the beginning of a scope to lock it until the end of the scope (in an exception
safe way.)
  
  TODO: Resource could be an ABC, becuase there are other ways of implementing
resources, even in SDL.  Lock depends only on wait() and post(), and that's
really the essence of a mutex.

  Note: if you use tryWait() or waitTimeout(), be prepared to catch a possible
ResourceError exception.
      
*/
#ifndef PATTERN_SPACE_MUTEX_INCLUSION_GUARD
#define PATTERN_SPACE_MUTEX_INCLUSION_GUARD
#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>

namespace PatternSpace {
    
/*********************  Resource  *********************/
    class Resource {
    public:
        class ResourceError {};  // exception class
        
        // a default of one means one thread can have the lock at a time.
        Resource(int i=1) { sdl_sem = SDL_CreateSemaphore(i); }
        ~Resource() { SDL_DestroySemaphore(sdl_sem);   }
        
        void wait() { SDL_SemWait(sdl_sem); }
        void post() { SDL_SemPost(sdl_sem); }
        int value() const { SDL_SemValue(sdl_sem); }
        
        bool tryWait()
        { 
            return boolOrError( SDL_SemTryWait(sdl_sem) ); 
        }
        
        bool waitTimeout(int timeout)
        { 
            return boolOrError( SDL_SemWaitTimeout(sdl_sem, timeout) ); 
        }
        
    private:
        SDL_sem * sdl_sem;
        // change the SDL error return code to a C++ exception.
        static bool boolOrError(Uint32 x) {
            if ( x == 0 ) return true;
            else if ( x == -1 ) throw ResourceError();
            else return false;
        }
    }; // end class Resource
    
/*********************  Lock  *********************/
    // RAII for a lock on a Resources
    class Lock {
    public:
        explicit Lock(Resource& target):
            resource(target) {
            resource.wait();  // get the resource
        }
        ~Lock() { 
            resource.post();  // release the resource
        }

    private:
        Resource& resource;
        //declare copy and assignment private; Locks shouldn't be copied
        Lock& operator=(Lock&);
        Lock(Lock&);

    }; // end class Lock
    
} // end namespace PatternSpace
#endif  // PATTERN_SPACE_MUTEX_INCLUSION_GUARD
