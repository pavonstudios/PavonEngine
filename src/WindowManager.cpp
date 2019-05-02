#include "WindowManager.hpp"
#include "engine.h"

#ifdef ES2
#ifdef ANDROID

#else
#include "engine.h"
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
 
   egl_surface = eglCreateWindowSurface ( egl_display, ecfg, x_window, NULL );
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



void WindowManager::create_window_xorg(){
       ///////  the X11 part  //////////////////////////////////////////////////////////////////
   // in the first part the program opens a connection to the X11 window manager
   //
 
   x_display = XOpenDisplay ( NULL );   // open the standard display (the primary screen)
   if ( x_display == NULL ) {
      cerr << "cannot connect to X server" << endl;
      
   }
 
   x_window  =  DefaultRootWindow( x_display );   // get the root window (usually the whole screen)
 
   XSetWindowAttributes  swa;
   swa.event_mask  =  ExposureMask | PointerMotionMask | KeyPressMask | KeyReleaseMask | StructureNotifyMask | ButtonPressMask;
 
   x_window  =  XCreateWindow (   // create a window with the provided parameters
              x_display, x_window,
              0, 0, this->window_width, this->window_height,   0,
              CopyFromParent, InputOutput,
              CopyFromParent, CWEventMask,
              &swa ); 

   XMapWindow ( x_display , x_window );             // make the window visible on the screen
   XStoreName ( x_display , x_window , this->window_name.c_str() ); // give the window a name 

   Atom wmDeleteMessage = XInternAtom(x_display, "WM_DESTROY_WINDOW", False);
   XSetWMProtocols(x_display, x_window, &wmDeleteMessage, 1);

   configure_egl();
    
}

void WindowManager::clear(){
      eglDestroyContext ( egl_display, egl_context );
      eglDestroySurface ( egl_display, egl_surface );
      eglTerminate      ( egl_display );
      XDestroyWindow    ( x_display, x_window );
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
		
		glfw_window = glfwCreateWindow(this->window_width, this->window_height, this->window_name.c_str(), nullptr, nullptr);
		if( glfw_window == NULL ){
			fprintf( stderr, "Failed to open GLFW window\n" );
			glfwTerminate();
			return;
		}
}
void WindowManager::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
   Engine* engine = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(window));
   engine->renderer.framebufferResized = true;
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

#ifdef ANDROID
   #include <android/log.h>
   #include "android_helper.h"

	void WindowManager::create_window(android_app *pApp) {
		display = eglGetDisplay(EGL_DEFAULT_DISPLAY);


		eglInitialize(display, NULL, NULL);

		/* get an appropriate EGL frame buffer configuration */
		eglChooseConfig(display, attribute_list, &config, 1, &num_config);


		context = eglCreateContext(display, config, EGL_NO_CONTEXT, GiveMeGLES2);


		surface = eglCreateWindowSurface(display, config, pApp->window, NULL);

      EGLint w = 0;
      EGLint h = 0;

		eglQuerySurface(display, surface, EGL_WIDTH, &w);

      eglQuerySurface(display, surface, EGL_HEIGHT, &h);

        LOGW("windows size: %f %f",(float)w,(float)h);

        this->window_width = float(w);
        this->window_height = float(h);

        this->update_window_size();

      eglMakeCurrent(display, surface, surface, context);

	}
	
#endif

void WindowManager::swap_buffers(){
      #ifdef ANDROID
         eglSwapBuffers(display, surface);
      #endif
      #ifdef ES2
         eglSwapBuffers ( egl_display, egl_surface );
      #endif
      #ifdef VULKAN
         glfwSwapBuffers(glfw_window);
      #endif
}

WindowManager::~WindowManager(){
   #ifdef VULKAN
   glfwDestroyWindow(glfw_window);

   glfwTerminate();
   #endif
}

void WindowManager::update_window_size(){
	int width = 0, height = 0;
   #ifdef VULKAN
        while (width == 0 || height == 0) {
            glfwGetFramebufferSize(glfw_window, &width, &height);
            glfwWaitEvents();
        }
   #endif
   if(width != 0 && height != 0){
      window_width = width;
	   window_height = height;
   }
	
   engine->main_camera.screen_width = window_width;
   engine->main_camera.screen_height = window_height;
	engine->update_render_size();
   
}

bool WindowManager::window_should_close(){
   bool value = false; 
   #ifdef VULKAN
      value = glfwWindowShouldClose(glfw_window);
   #endif
   #if defined(ES2)
      value = false;
   #endif
   return value;
}
void WindowManager::check_events(){
   #ifdef ES2
      while ( XPending ( x_display ) ){
         XEvent  xev;
         KeySym key;		
         char text[255];	
         KeySym key_release;		
         char key_release_char[255];	
         XNextEvent( x_display, &xev );

         if ( xev.type == KeyPress ){
            //std::cout << "key pressed \n";
         } 
         if (xev.type == KeyRelease){
         // std::cout << "key realease from window manager \n";
         }

         if ( xev.type == MotionNotify ) {  // if mouse has moved

            //cout << "move to: "<< xev.xmotion.x << "," << xev.xmotion.y << endl;
            //engine->input.move_camera = true;
            engine->input.mouse_movement(engine,xev.xmotion.x,xev.xmotion.y);
         }
            
         if(xev.type == ConfigureNotify){
            XConfigureEvent xce = xev.xconfigure;
            this->window_width = xce.width;
            this->window_height = xce.height;
            update_window_size();           
         }

         /* use the XLookupString routine to convert the invent
               KeyPress data into regular text.  Weird but necessary...
            */
         if (xev.type==KeyPress&&
               XLookupString(&xev.xkey,text,255,&key,0)==1) {
            
               if (text[0]=='q') {
                  
               }
               engine->input.key_verifier_pressed(text[0]);

            }
         if (xev.type==KeyRelease &&
               XLookupString(&xev.xkey,key_release_char,255,&key_release,0)==1) {

               engine->input.key_verifier_released(key_release_char[0]);
            }
         engine->input.left_button_pressed = false;
         if(xev.type == ButtonPress){
            
            if(xev.xbutton.button == Button1){
               //std::cout << "left click \n";
               //std::cout << xev.xbutton.x << " " << xev.xbutton.y << std::endl;
               engine->input.left_button_pressed = true;
            }
         }

         if(xev.type == ClientMessage && xev.xclient.data.l[0] == wmDeleteMessage){
            std::cout << "close message\n";
            
         }
      }
      
     
   #endif
   #ifdef VULKAN
      	glfwPollEvents();
   #endif
}