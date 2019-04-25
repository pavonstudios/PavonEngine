#ifndef WINDOW
#define WINDOW

#ifdef ES2
    #include  <X11/Xlib.h>
    #include  <X11/Xatom.h>
    #include  <X11/Xutil.h>
    
    #include <EGL/egl.h>
    #include <GLES2/gl2.h>
    using namespace std;
#endif
#ifdef ANDROID
    #include <EGL/egl.h>
    #include <GLES2/gl2.h>
    #include <android_native_app_glue.h>
#endif
#ifdef VULKAN
    #include "GLFW/glfw3.h"
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
    void create_window();
    void swap_buffers();
    bool window_should_close();
    void check_events();
    ~WindowManager();
    #ifdef VULKAN
        GLFWwindow* get_window(){
            return glfw_window;
        }
    #endif
    #ifdef ES2
       
        #ifdef ANDROID
            void create_window(android_app * app);
        #else
            Display    *x_display;
            Window      x_window;
            Atom wmDeleteMessage;
            EGLDisplay  egl_display;
            EGLContext  egl_context;
            EGLSurface  egl_surface;
                        
            void create_window_xorg();
            void clear();
            
            
        private:
            void configure_egl();
        #endif
    #endif
    #ifdef VULKAN
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