#include "WindowManager.hpp"

#ifdef ES2
#ifdef ANDROID

#else
void WindowManager::configure_egl(){
    ///////  the egl part  //////////////////////////////////////////////////////////////////
   //  egl provides an interface to connect the graphics related functionality of openGL ES
   //  with the windowing interface and functionality of the native operation system (X11
   //  in our case.
 
   egl_display  =  eglGetDisplay( (EGLNativeDisplayType) x_display );
   if ( egl_display == EGL_NO_DISPLAY ) {
      cerr << "Got no EGL display." << endl;
      
   }
 
   if ( !eglInitialize( egl_display, NULL, NULL ) ) {
      cerr << "Unable to initialize EGL" << endl;
     
   }
 
   EGLint attr[] = {       // some attributes to set up our egl-interface
      EGL_BUFFER_SIZE, 16,
      EGL_RENDERABLE_TYPE,
      EGL_OPENGL_ES2_BIT,
      EGL_DEPTH_SIZE, 24,
      EGL_NONE
   };
 
   EGLConfig  ecfg;
   EGLint     num_config;
   if ( !eglChooseConfig( egl_display, attr, &ecfg, 1, &num_config ) ) {
      cerr << "Failed to choose config (eglError: " << eglGetError() << ")" << endl;
      
   }
 
   if ( num_config != 1 ) {
      cerr << "Didn't get exactly one config, but " << num_config << endl;
    
   }
 
   egl_surface = eglCreateWindowSurface ( egl_display, ecfg, win, NULL );
   if ( egl_surface == EGL_NO_SURFACE ) {
      cerr << "Unable to create EGL surface (eglError: " << eglGetError() << ")" << endl;
      
   }
 
   //// egl-contexts collect all state descriptions needed required for operation
   EGLint ctxattr[] = {
      EGL_CONTEXT_CLIENT_VERSION, 2,
      EGL_NONE
   };
   egl_context = eglCreateContext ( egl_display, ecfg, EGL_NO_CONTEXT, ctxattr );
   if ( egl_context == EGL_NO_CONTEXT ) {
      cerr << "Unable to create EGL context (eglError: " << eglGetError() << ")" << endl;
      
   } 
   //// associate the egl-context with the egl-surface
   eglMakeCurrent( egl_display, egl_surface, egl_surface, egl_context );
}

void WindowManager::swap_buffers(){

      eglSwapBuffers ( egl_display, egl_surface );

}

void WindowManager::create_window_xorg(){
       ///////  the X11 part  //////////////////////////////////////////////////////////////////
   // in the first part the program opens a connection to the X11 window manager
   //
 
   x_display = XOpenDisplay ( NULL );   // open the standard display (the primary screen)
   if ( x_display == NULL ) {
      cerr << "cannot connect to X server" << endl;
      
   }
 
   Window root  =  DefaultRootWindow( x_display );   // get the root window (usually the whole screen)
 
   XSetWindowAttributes  swa;
   swa.event_mask  =  ExposureMask | PointerMotionMask | KeyPressMask;
 
   win  =  XCreateWindow (   // create a window with the provided parameters
              x_display, root,
              0, 0, 800, 480,   0,
              CopyFromParent, InputOutput,
              CopyFromParent, CWEventMask,
              &swa );
 
 
   XMapWindow ( x_display , win );             // make the window visible on the screen
   XStoreName ( x_display , win , "GL test" ); // give the window a name
 
   Atom wm_state   = XInternAtom ( x_display, "_NET_WM_STATE", False );
 
   XEvent xev;
   memset ( &xev, 0, sizeof(xev) );
 
   xev.type                 = ClientMessage;
   xev.xclient.window       = win;
   xev.xclient.message_type = wm_state;
   xev.xclient.format       = 32;

   XSendEvent (                // send an event mask to the X-server
      x_display,
      DefaultRootWindow ( x_display ),
      False,
      SubstructureNotifyMask,
      &xev );

   configure_egl();
   const float
      window_width  = 800.0,
      window_height = 480.0;
 
}

void WindowManager::clear(){
      eglDestroyContext ( egl_display, egl_context );
      eglDestroySurface ( egl_display, egl_surface );
      eglTerminate      ( egl_display );
      XDestroyWindow    ( x_display, win );
      XCloseDisplay     ( x_display );
}
#endif

#endif
#ifdef VULKAN
void WindowManager::create_window_glfw(){
   	if( !glfwInit() )
		{
			fprintf( stderr, "Failed to initialize GLFW\n" );
			return;
		}	
	
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		
		window = glfwCreateWindow(800, 600, "Engine", nullptr, nullptr);
		if( window == NULL ){
			fprintf( stderr, "Failed to open GLFW window\n" );
			glfwTerminate();
			return;
		}
}
#endif

void WindowManager::create_window(){
   #ifdef ES2
      create_window_xorg();
   #endif
   #ifdef VULKAN
      create_window_glfw();
   #endif
  

}