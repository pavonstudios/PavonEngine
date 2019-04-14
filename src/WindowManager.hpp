#ifndef WINDOW
#define WINDOW

#ifdef ES2
    #include  <X11/Xlib.h>
    #include  <X11/Xatom.h>
    #include  <X11/Xutil.h>
   #include  <iostream>
    #include  <cstdlib>
    #include  <cstring>
    #include <EGL/egl.h>
    #include <GLES2/gl2.h>
    using namespace std;
#endif

class WindowManager{
public:
    
    #ifdef ES2

       
        #ifdef ANDROID
            void create_window(android_app * app);
        #else
            Display    *x_display;
            Window      win;
            EGLDisplay  egl_display;
            EGLContext  egl_context;
            EGLSurface  egl_surface;
            void create_window();
            void clear();
        private:
            void configure_egl();
        #endif
    #endif

};

#endif