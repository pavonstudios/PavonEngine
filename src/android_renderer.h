#ifndef _Android_Renderer_H_
#define _Android_Renderer_H_

#include <initializer_list>
#include <memory>
#include <cstdlib>
#include <cstring>
#include <jni.h>
#include <errno.h>
#include <cassert>

#include <EGL/egl.h>
#include <GLES/gl.h>

//#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

extern NativeWindowType createNativeWindow(void);
static EGLint const attribute_list[] = {
        EGL_RED_SIZE, 1,
        EGL_GREEN_SIZE, 1,
        EGL_BLUE_SIZE, 1,
        EGL_NONE
};

class Renderer{
public:


    Renderer(android_app *pApp){
        app = pApp;
        init();
        //init(&engine);
        LOGW("Hello android");

    };
    void render(){
        //LOGW("rendering");
        glClearColor(1.0, 1.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glFlush();

        eglSwapBuffers(display, surface);
    };
private:
    EGLDisplay display;
    EGLConfig config;
    EGLContext context;
    EGLSurface surface;
    NativeWindowType native_window;
    EGLint num_config;
    struct android_app * app;

    void init(){

        /* get an EGL display connection */
        display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

        /* initialize the EGL display connection */
        eglInitialize(display, NULL, NULL);

        /* get an appropriate EGL frame buffer configuration */
        eglChooseConfig(display, attribute_list, &config, 1, &num_config);

        /* create an EGL rendering context */
        context = eglCreateContext(display, config, EGL_NO_CONTEXT, NULL);

        /* create a native window */
        //native_window = createNativeWindow();

        /* create an EGL window surface */
        surface = eglCreateWindowSurface(display, config, app->window, NULL);

        /* connect the context to the surface */
        eglMakeCurrent(display, surface, surface, context);


    };

};

#endif