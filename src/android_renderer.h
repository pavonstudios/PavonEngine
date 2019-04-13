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



#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <chrono>

#include "android_helper.h"

extern NativeWindowType createNativeWindow(void);

void
print_shader_info_log (
        GLuint  shader      // handle to the shader
)
{
    GLint  length;

    glGetShaderiv ( shader , GL_INFO_LOG_LENGTH , &length );

    if ( length ) {
        char* buffer  =  new char [ length ];
        glGetShaderInfoLog ( shader , length , NULL , buffer );
        LOGW("shader info %s",buffer);
        //cout << "shader info: " <<  buffer << flush;
        delete [] buffer;

        GLint success;
        glGetShaderiv( shader, GL_COMPILE_STATUS, &success );
        if ( success != GL_TRUE )   exit ( 1 );
    }
}

GLuint
load_shader (
        const char  *shader_source,
        GLenum       type
)
{
    GLuint  shader = glCreateShader( type );

    glShaderSource  ( shader , 1 , &shader_source , NULL );
    glCompileShader ( shader );

    print_shader_info_log ( shader );

    return shader;
}




#include "engine.h"
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
       // init();
        engine.create_window(pApp);

        init_gl();
        

    };
   
private:

    struct android_app * app;

    GLuint vertexbuffer;
    GLuint VertexArrayID;

    GLuint vertex_array_id;

    GLuint vertex_buffer;
    GLuint indices;

    GLint textureid;

    GLint
            phase_loc,
            offset_loc,
            position_loc,
            sampler,
            uvposition,
            mvp_loc;

    GLuint shaderProgram;

    void load_shader_from_file(){
        AAsset* file = AAssetManager_open(app->activity->assetManager,"vert.glsl", AASSET_MODE_BUFFER);

        size_t file_length = AAsset_getLength(file);
        char* fileContent = new char[file_length+1];

        AAsset_read(file, fileContent,file_length);


        GLuint vertexShader   = load_shader ( fileContent , GL_VERTEX_SHADER  );     // load vertex shader
        GLuint fragmentShader = load_shader ( fragment_src , GL_FRAGMENT_SHADER );  // load fragment shader

        LOGW("Shaders loaded");
        shaderProgram = glCreateProgram ();                 // create program object
        glAttachShader ( shaderProgram, vertexShader );             // and attach both...
        glAttachShader ( shaderProgram, fragmentShader );           // ... shaders to it

        glLinkProgram ( shaderProgram );    // link the program
        glUseProgram  ( shaderProgram );    // and select it for usage
    }

    void init_gl(){
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);


        LOGW("Loading shaders........................");

        load_shader_from_file();






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

        //// now get the locations (kind of handle) of the shaders variables
        position_loc  = glGetAttribLocation  ( shaderProgram , "position" );
        uvposition         = glGetAttribLocation( shaderProgram , "UV" );

        mvp_loc         = glGetUniformLocation( shaderProgram , "MVP");
        sampler         = glGetUniformLocation( shaderProgram , "texture_sampler");

        LOGW("GGetting shader attribute location");
    }

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



        glVertexAttribPointer ( position_loc, 3, GL_FLOAT, false, sizeof(Vertex), (void*)0 );
        glVertexAttribPointer ( uvposition, 2, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex,texCoord) );


        glEnableVertexAttribArray ( position_loc );
        glEnableVertexAttribArray ( sampler );

      //  glUniform3()f(textureid,sampler);
        glBindTexture(GL_TEXTURE_2D,textureid);
        glDrawElements(GL_TRIANGLES,meshes[0]->indices.size(),GL_UNSIGNED_INT,(void*)0);
        
        eglSwapBuffers(engine.display, engine.surface);
    };

};

#endif