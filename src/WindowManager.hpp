#ifndef WINDOW
#define WINDOW

#if defined(LINUX) && defined (ES2)
    #include  <X11/Xlib.h>
    #include  <X11/Xatom.h>
    #include  <X11/Xutil.h>
    #include <GLES2/gl2.h>
   
#endif
 #if defined(LINUX) && defined(X11) && defined (ES2)
    #include <EGL/egl.h>
  
    using namespace std;
#endif
#ifdef ANDROID
    #include <EGL/egl.h>
    #include <GLES2/gl2.h>
    #include <android_native_app_glue.h>
#endif
#if defined (VULKAN) || defined (WINDOWS)
    #include <GLFW/glfw3.h>
#endif
#ifdef WAYLAND
    #include <wayland-client.h>
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
    std::string window_name = "Engine";
    bool close_window = false;
    void create_window();
    void swap_buffers();
    bool window_should_close();
    void check_events();
    ~WindowManager();
    void update_window_size();
    void create_wayland_window();
	static void error_callback(int error, const char* description);
   
    #ifdef ES2
       
        #ifdef ANDROID
            void create_window(android_app * app);
        #endif
        
        #ifdef X11
            Display    *x_display;
            Window      x_window;
            Atom wmDeleteMessage;
            
                        
            void create_window_xorg();
            void clear();
            void move_cursor_to_center();            
        #endif
        #if defined(LINUX) && defined(X11) && defined (ES2)
        EGLDisplay  egl_display;
        EGLContext  egl_context;
        EGLSurface  egl_surface;
        private:
            void configure_egl();
        #endif
    #endif
    #ifdef WINDOWS
    GLFWwindow* glfw_window;
    #endif
    #ifdef VULKAN
    GLFWwindow* get_window(){
        return glfw_window;
    }
    public:
        static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
    private:
        GLFWwindow* glfw_window;
        void create_window_glfw();
        
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