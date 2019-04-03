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
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

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

        LOGW("Hello android");

    };
    void render(){
        //LOGW("rendering");
        //glViewport(0,0,800,600);

        glClearColor(1.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        //glFlush();

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
        );
       
       // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
       // glEnableVertexAttribArray(0);
       
        glDrawArrays(GL_TRIANGLES, 0, 3);

        
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

    GLuint vertexbuffer;
    GLuint VertexArrayID;

     GLfloat vVertices[9] = {0.0f, 0.5f, 0.0f,
                               -0.5f, -0.5f, 0.0f,
                               0.5f, -0.5f, 0.0f};
                               
    void init(){

        /* get an EGL display connection */
        display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

        /* initialize the EGL display connection */
        eglInitialize(display, NULL, NULL);

        /* get an appropriate EGL frame buffer configuration */
        eglChooseConfig(display, attribute_list, &config, 1, &num_config);

        /* create an EGL rendering context */
        context = eglCreateContext(display, config, EGL_NO_CONTEXT, NULL);       

        /* create an EGL window surface */
        surface = eglCreateWindowSurface(display, config, app->window, NULL);

        /* connect the context to the surface */
        eglMakeCurrent(display, surface, surface, context);

        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);

        glGenBuffers(1, &vertexbuffer);

        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

        glBufferData(GL_ARRAY_BUFFER, sizeof(vVertices), vVertices, GL_STATIC_DRAW);

    };

};

#endif