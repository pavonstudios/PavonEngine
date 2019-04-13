#ifndef _Android_Renderer_H_
#define _Android_Renderer_H_

#include <initializer_list>
#include <memory>
#include <cstdlib>
#include <cstring>
#include <jni.h>
#include <errno.h>
#include <cassert>
#include <vector>

#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include <android/log.h>
#include <android_native_app_glue.h>
#include "3D_objects.h"


#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <chrono>

#include "android_helper.h"

extern NativeWindowType createNativeWindow(void);

//#include "engine.h"
#include "asset_manager.h"

using  namespace engine;
using namespace glm;
class Renderer{
public:
   std::vector<EMesh*> meshes;
   Engine engine;
    Renderer(android_app *pApp){
        app = pApp;
        LOGW("Initialiazing");
        init();

        

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

    GLuint vertex_array_id;

    GLuint vertex_buffer;
    GLuint indices;

    GLint textureid;

     GLfloat vVertices[9] = {0.0f, 0.5f, 0.0f,
                               -0.5f, -0.5f, 0.0f,
                               0.5f, -0.5f, 0.0f};
                               
    void init(){

        
        display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

        
        eglInitialize(display, NULL, NULL);

        /* get an appropriate EGL frame buffer configuration */
        eglChooseConfig(display, attribute_list, &config, 1, &num_config);

       
        context = eglCreateContext(display, config, EGL_NO_CONTEXT, GiveMeGLES2);       

        
        surface = eglCreateWindowSurface(display, config, app->window, NULL);

     
        eglMakeCurrent(display, surface, surface, context);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);


        LOGW("Loading shaders........................");

       

        GLuint vertexShader   = load_shader ( vertex_src , GL_VERTEX_SHADER  );     // load vertex shader
        GLuint fragmentShader = load_shader ( fragment_src , GL_FRAGMENT_SHADER );  // load fragment shader
        
         LOGW("Shaders loaded");
        GLuint shaderProgram  = glCreateProgram ();                 // create program object
        glAttachShader ( shaderProgram, vertexShader );             // and attach both...
        glAttachShader ( shaderProgram, fragmentShader );           // ... shaders to it
        
        glLinkProgram ( shaderProgram );    // link the program
        glUseProgram  ( shaderProgram );    // and select it for usage
        
        //// now get the locations (kind of handle) of the shaders variables
        position_loc  = glGetAttribLocation  ( shaderProgram , "position" );
        uvposition         = glGetAttribLocation( shaderProgram , "coord" );


        phase_loc     = glGetUniformLocation ( shaderProgram , "phase"    );
        offset_loc    = glGetUniformLocation ( shaderProgram , "offset"   ); 
        mvp_loc         = glGetUniformLocation( shaderProgram , "MVP");

         sampler         = glGetUniformLocation( shaderProgram , "texture_sampler");


      LOGW("Loading shaders........................");


/*         AAsset* file = AAssetManager_open(super_asset_manager,"openme.txt", AASSET_MODE_BUFFER);

        size_t file_length = AAsset_getLength(file);
        char* fileContent = new char[file_length+1];

        AAsset_read(file, fileContent,file_length);

        __android_log_print(ANDROID_LOG_WARN,"native-activity","%s",fileContent);

            EMesh * mesh = new EMesh();
            if(mesh->load_model_gltf("skydome.gltf") == -1 ){
                      __android_log_print(ANDROID_LOG_WARN,"native-activity","%s","error loading model");

            }else{
               __android_log_print(ANDROID_LOG_WARN,"native-activity","%s","OK GLTF object loaded");
            } */

      EMesh* mesh = new EMesh();
      if(mesh->load_mode_gltf_android("police_patrol.gltf",app->activity->assetManager) == -1){

          __android_log_print(ANDROID_LOG_WARN,"native-activity","%s","error loading model");

      }else{
          __android_log_print(ANDROID_LOG_WARN,"native-activity","%s","OK GLTF object loaded");
      }
        meshes.push_back(mesh);
      LOGW("Generating vertex buffer");





        glGenBuffers(1,&vertex_buffer);
      glBindBuffer(GL_ARRAY_BUFFER,vertex_buffer);
      glBufferData(GL_ARRAY_BUFFER,mesh->vertices.size() * sizeof(Vertex),mesh->vertices.data(),GL_STATIC_DRAW);
        LOGW("OK Vertex data generated");

      LOGW("Generating index buffer");


        glGenBuffers(1,&indices);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,indices);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,mesh->indices.size() * sizeof(unsigned int),mesh->indices.data(), GL_STATIC_DRAW);


        LOGW("Generating image buffer");
        AssetManager assets;
        textureid = assets.load_bmp("patrol.bmp",app->activity->assetManager);

       glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,textureid);


    };

   public:
       void render(){
        //LOGW("rendering");
        //glViewport(0,0,800,600);

        glClearColor(1.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glFlush();

      static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        glm::mat4 model = glm::mat4(1.0);
        glm::mat4 Projection = glm::perspective(glm::radians(45.f), 768.f/1280.f, 0.01f, 1000.f);
        glm::mat4 view = glm::lookAt(vec3(0,15,0),vec3(0,0,0),vec3(0,0,1));

      model = glm::rotate(model, time * glm::radians(12.f), glm::vec3(0.0f, 1.0f, 1.0f));

        mat4 mvp = Projection * view * model;

        glUniformMatrix4fv(mvp_loc,1,GL_FALSE,&mvp[0][0]);




       /*  glVertexAttribPointer ( position_loc, 3, GL_FLOAT, false, 0, vertexArray );
            glEnableVertexAttribArray ( position_loc );
            glDrawArrays ( GL_TRIANGLE_STRIP, 0, 5 );*/




        glVertexAttribPointer ( position_loc, 3, GL_FLOAT, false, sizeof(Vertex), (void*)0 );
        glVertexAttribPointer(
                sampler,
                2,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                sizeof(Vertex),                  // stride
                (void*)offsetof(Vertex,texCoord)            // array buffer offset
        );


        glEnableVertexAttribArray ( position_loc );
        glEnableVertexAttribArray ( sampler );

        //glUniform1f(textureid,sampler);
        glDrawElements(GL_TRIANGLES,meshes[0]->indices.size(),GL_UNSIGNED_INT,(void*)0);
        
        eglSwapBuffers(display, surface);
    };

};

#endif