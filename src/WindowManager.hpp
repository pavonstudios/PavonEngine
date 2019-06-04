#ifndef WINDOW
#define WINDOW

#if defined(X11)
    #include  <X11/Xlib.h>
    #include  <X11/Xatom.h>
    #include  <X11/Xutil.h>     
#endif

#ifdef GLX
    #define GL_GLEXT_PROTOTYPES
    #define GLX_GLXEXT_PROTOTYPES
    #include <GL/glx.h> 
    #include <GL/glxext.h>
#endif // GLX

#ifdef ES2
    #include <GLES2/gl2.h>
#endif

#if defined (EGL)
    #include <EGL/egl.h>  
    using namespace std;
#endif

#ifdef ANDROID
    #include <EGL/egl.h>
    #include <GLES2/gl2.h>
    #include <android_native_app_glue.h>
#endif

#if defined (GLFW) 
	#define GLFW_INCLUDE_NONE
    #include <GLFW/glfw3.h>
#endif

#ifdef WAYLAND
    #include <wayland-client.h>
    #include <wayland-server.h>
    #include "WindowManager/xdg-shell-protocol.h"
    #include "WindowManager/wayland-xdg-shell-client-protocol.h"
    #include <wayland-client-protocol.h>
    #include <dlfcn.h>
    #include <EGL/egl.h>
    typedef struct wl_egl_window* (* PFN_wl_egl_window_create)(struct wl_surface*, int, int);
#endif

#ifdef WINDOWS
    #include <Windows.h>
    #include <tchar.h>
#endif

#include  <iostream>
#include  <cstdlib>
#include  <cstring>

class Engine;

class WindowManager{
public:
    Engine* engine;
    int window_width = 800;
    int window_height = 600;
    const char* window_name = "Engine";
    bool close_window = false;
    void create_window();
    void swap_buffers();
    bool window_should_close();
    void check_events();
	
    ~WindowManager();
    void update_window_size();
    void create_wayland_window();
    void clear();
	static void error_callback(int error, const char* description);
	
	

	#ifdef WINDOWS
		static Engine* static_engine_pointer;
		HWND window_handler;
		HINSTANCE window_instance;
		void create_window_windows(HINSTANCE hInstance);
		static LRESULT CALLBACK WndProc(
			_In_ HWND   hwnd,
			_In_ UINT   uMsg,
			_In_ WPARAM wParam,
			_In_ LPARAM lParam
		);
		void window_resize();
	#endif 

	#ifdef OPENGL
		HGLRC rendering_context;
		HDC device_context;
		void prepare_window_to_opengl();
	#endif // OPENGL    
    
       
    #ifdef ANDROID
        void create_window(android_app * app);
    #endif
        
    #ifdef WAYLAND
        struct wl_display* display = nullptr;
        struct xdg_surface *xdg_surface = nullptr;        
        struct wl_surface *surface;
        struct wl_egl_window* egl_window;
    #endif

    #ifdef X11
        Display    *x_display;
        Window      x_window;
        Atom wmDeleteMessage;
        
                    
        void create_window_xorg();
        
        void move_cursor_to_center();            
    #endif
    #if defined (EGL)
    EGLDisplay  egl_display;
    EGLContext  egl_context;
    EGLSurface  egl_surface;
    private:
        void configure_egl();
    #endif
   
   


	#if defined (GLFW)
    GLFWwindow* get_window(){
        return glfw_window;
    }
    public:
        static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
        void create_window_glfw();
    private:
        GLFWwindow* glfw_window;
        
        
    #endif
    #ifdef ANDROID
        EGLDisplay display;
        EGLConfig config;
        EGLContext context;
        EGLSurface surface;
        EGLint num_config;
        void create_window(android_app *pApp);
        
    #endif
};

#endif