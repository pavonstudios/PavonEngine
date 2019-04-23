#ifndef _Android_Renderer_H_
#define _Android_Renderer_H_

#include <initializer_list>
#include <memory>
#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>

#include <errno.h>
#include <cassert>
#include <vector>


#include <GLES2/gl2.h>

#ifdef ANDROID
    #include <android/log.h>
    #include <android_native_app_glue.h>
    #include <jni.h>
    #include "glm/glm.hpp"
    #include "glm/gtc/matrix_transform.hpp"

#endif
#include "3D_objects.h"



#include <chrono>

#include "android_helper.h"



#include "asset_manager.h"

using  namespace engine;
using namespace std;
using namespace glm;
class Renderer{
public:
    bool bReady = false;
private:
    GLuint vertexbuffer;

    GLuint vertex_buffer;
    GLuint indices;

    GLuint textureid;
    GLuint texture2id;

    GLuint shaderProgram;

    GLuint load_shader (const char  *shader_source, GLenum type)
    {
        GLuint  shader = glCreateShader( type );

        glShaderSource  ( shader , 1 , &shader_source , NULL );
        glCompileShader ( shader );

        print_shader_info_log ( shader );

        return shader;
    }

    void print_shader_info_log (GLuint  shader){
        GLint  length;

        glGetShaderiv ( shader , GL_INFO_LOG_LENGTH , &length );

        if ( length ) {
            char* buffer  =  new char [ length ];
            glGetShaderInfoLog ( shader , length , NULL , buffer );
            #ifdef ANDROID
                LOGW("shader info %s",buffer);
            #else
                cout << "shader info: " <<  buffer << flush;
            #endif
            delete [] buffer;

            GLint success;
            glGetShaderiv( shader, GL_COMPILE_STATUS, &success );
            if ( success != GL_TRUE )   exit ( 1 );
        }
    }
    
    char* load_shader_file(const char* path){
        #ifdef ANDROID
            AAsset* file = AAssetManager_open(app->activity->assetManager,path, AASSET_MODE_BUFFER);

            size_t file_length = AAsset_getLength(file);
            char* fileContent = new char[file_length+1];

            AAsset_read(file, fileContent,file_length);
            AAsset_close(file);
            return fileContent;
        #else
        std::string spath = path;
         std::string content;
            std::ifstream fileStream(spath, std::ios::in);

            if(!fileStream.is_open()) {
                std::string error = "Could not read file " + spath;
                throw std::runtime_error(error);
                
            }

            std::string line = "";
            while(!fileStream.eof()) {
                std::getline(fileStream, line);
                content.append(line + "\n");
            }

            fileStream.close();
             int n = content.length(); 
             char* char_content = new char[n+1];
             strcpy(char_content,content.c_str());

            return char_content;
        #endif
    }
    
    void load_shaders(){

        #ifdef ANDROID
            char* vertex_shader_src = load_shader_file("vert.glsl");
            char* fragment_shader_src = load_shader_file("frag_vert_color.glsl");
        #else
            char* vertex_shader_src = load_shader_file("android/app/src/main/assets/vert.glsl");
            char* fragment_shader_src = load_shader_file("android/app/src/main/assets/frag.glsl");
        #endif      

        GLuint vertexShader   = load_shader ( vertex_shader_src , GL_VERTEX_SHADER  );     // load vertex shader
        GLuint fragmentShader = load_shader ( fragment_shader_src , GL_FRAGMENT_SHADER );  // load fragment shader

        
        shaderProgram = glCreateProgram ();                 // create program object
        glAttachShader ( shaderProgram, vertexShader );             // and attach both...
        glAttachShader ( shaderProgram, fragmentShader );           // ... shaders to it

        glLinkProgram ( shaderProgram );    // link the program
       
    }
     
public:
#ifdef ES2
    void load_shaders(EMesh* mesh){

        char* vertex_shader_src = load_shader_file(mesh->data.vertex_shader_path.c_str());
        char* fragment_shader_src = load_shader_file(mesh->data.fragment_shader_path.c_str());
         

        GLuint vertexShader   = load_shader ( vertex_shader_src , GL_VERTEX_SHADER  );     // load vertex shader
        GLuint fragmentShader = load_shader ( fragment_shader_src , GL_FRAGMENT_SHADER );  // load fragment shader

        
        mesh->shader_program = glCreateProgram ();                 // create program object
        glAttachShader ( mesh->shader_program, vertexShader );             // and attach both...
        glAttachShader ( mesh->shader_program, fragmentShader );           // ... shaders to it

        glLinkProgram ( mesh->shader_program  );    // link the program
       
    }

#endif
    void init_gl(){
        //glViewport(0,0,800,600);

        glClearColor(0.2, 0.0, 0.0, 1.0);
        
       
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);           

        
        #ifdef ANDROID
            load_shaders();
            //init_3d_model();
            create_triangule();
            create_texture();
        #endif


    }
    void create_triangule(){
        

        EMesh* triangle = new EMesh();
        glm::vec2 texcoor = glm::vec2(0.f);
        glm::vec4 posss = glm::vec4(0.f);
        glm::vec4 witjj = glm::vec4(0.f);
        Vertex vert1{};
        vert1.pos = glm::vec3(1.0,1.0,0.0);
        Vertex vert2{};
        vert2.pos = glm::vec3(0.0,0.0,0.0);
        Vertex vert3{};
        vert3.pos = glm::vec3(0.0,1.0,0.0);

        vert1.texCoord = glm::vec2(1.0f,1.0f);
        vert1.color = vec3(1.0,0.0,0.0);
        vert1.joint0 = glm::vec4(0.0f);
        vert1.weight0 = glm::vec4(0.0f);

           vert3.texCoord = glm::vec2(0.0f,0.0f);
        vert3.joint0 = glm::vec4(0.0f);
         vert3.color = vec3(0.0,0.0,1.0);
        vert3.weight0 = glm::vec4(0.0f);

           vert2.texCoord = glm::vec2(1.0f,0.0f);
        vert2.joint0 = glm::vec4(0.0f);
         vert2.color = vec3(0.0,1.0,0.0);
        vert2.weight0 = glm::vec4(0.0f);

        triangle->vertices.push_back(vert1);
        triangle->vertices.push_back(vert2);
        triangle->vertices.push_back(vert3);

                float vertices[] = {
            // positions          // colors           // texture coords
            -0.5f,  0.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
            0.5f, 0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f   // bottom left
           // -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
        };

        glGenBuffers(1,&vertex_buffer);
        glBindBuffer(GL_ARRAY_BUFFER,vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER,triangle->vertices.size() * sizeof(Vertex),triangle->vertices.data(),GL_STATIC_DRAW);

        glVertexAttribPointer ( 0, 3, GL_FLOAT, false, sizeof(Vertex), (void*)0 );
        glEnableVertexAttribArray ( 0 ); 
        glVertexAttribPointer ( 1, 3, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex,color) );
        glEnableVertexAttribArray ( 1 );
        glVertexAttribPointer ( 2, 2, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex,texCoord) );
        glEnableVertexAttribArray ( 2 );
    }
  
    void init_3d_model(){
        EMesh* mesh = new EMesh();
        int mesh_load_result;
        #ifdef ANDROID
            mesh_load_result = mesh->load_mode_gltf_android("police_patrol.gltf",app->activity->assetManager);
        #else//ES2
            mesh_load_result = mesh->load_model_gltf("android/app/src/main/assets/police_patrol.gltf");
        #endif
    
       
        glGenBuffers(1,&vertex_buffer);
        glBindBuffer(GL_ARRAY_BUFFER,vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER,mesh->vertices.size() * sizeof(Vertex),mesh->vertices.data(),GL_STATIC_DRAW);

        glGenBuffers(1,&indices);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,indices);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,mesh->indices.size() * sizeof(unsigned int),mesh->indices.data(), GL_STATIC_DRAW);

        create_texture();       
       
        int vPos = glGetAttribLocation(shaderProgram,"position");
        int incolor = glGetAttribLocation(shaderProgram,"inColor");        
        int incood = glGetAttribLocation(shaderProgram,"inUV");
        int samplerid = glGetUniformLocation(shaderProgram, "texture_sampler");
         
        activate_vertex_attributes(mesh);
    }
    void activate_vertex_attributes(EMesh* mesh){
#ifdef ES2
        glBindBuffer(GL_ARRAY_BUFFER,mesh->vertex_buffer);
#endif
        glVertexAttribPointer ( 0, 3, GL_FLOAT, false, sizeof(Vertex), (void*)0 );
        glEnableVertexAttribArray ( 0 ); 

        glVertexAttribPointer ( 1, 3, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex,color) );
        glEnableVertexAttribArray ( 1 );            
        
        glVertexAttribPointer ( 2, 2, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex,texCoord) );
        glEnableVertexAttribArray ( 2 );

    }
    void create_texture(){
             

        #ifdef ANDROID
            AssetManager assets;
            image_size size = assets.load_bmp("police_patrol.pvn",app->activity->assetManager);            
        #else                   
            AssetManager assets;
            image_size size = assets.load_and_get_size("textures/skydome.jpg");      
        #endif
            glActiveTexture(GL_TEXTURE0);
            glGenTextures(1, &textureid);
            glBindTexture(GL_TEXTURE_2D,textureid);
                     
            //glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,size.width,size.heigth,0,GL_RGB,GL_UNSIGNED_BYTE,size.data);

            float pixels[] = {
            255.0f, 0.0f, 0.0f,1.0f ,   0.0f, 255.0f, 0.0f, 1.0f,
            0.0f, 255.0f, 0.0f,1.0f ,   0.0f, 0.0f, 255.0f,1.0f

            };
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_FLOAT, pixels);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            //glGenerateMipmap(GL_TEXTURE_2D);

    }
    
    #ifdef ANDROID
    struct android_app * app;
    #endif
#ifdef ES2
   public:
   void load_mesh_texture(EMesh* mesh){

        glActiveTexture(GL_TEXTURE0);
        glGenTextures(1, &mesh->texture_id);
        glBindTexture(GL_TEXTURE_2D,mesh->texture_id);


        AssetManager assets;
        #ifdef ANDROID
            
            image_size size = assets.load_bmp("police_patrol.pvn",app->activity->assetManager);    //TODO: load texture with android path        
        #else                   
            image_size size;
            if(mesh->texture.hasTexture){
                size.heigth = mesh->texture.height;
                size.width = mesh->texture.width;
                size.data = mesh->texture.data;
                glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,size.width,size.heigth,0,GL_RGBA,GL_UNSIGNED_BYTE,size.data);
            }else{
                size = assets.load_and_get_size(mesh->texture_path.c_str());
                glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,size.width,size.heigth,0,GL_RGB,GL_UNSIGNED_BYTE,size.data); 
            }
                
        #endif                   
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        //glGenerateMipmap(GL_TEXTURE_2D);
   }

#endif
       void render(){                    

            #ifdef ANDROID
                glUseProgram  ( shaderProgram );
                draw_mesh();   
//                engine.window_manager.swap_buffers();
            #endif

    };

    void update_mvp(){
       
        {  //mvp 
                static auto startTime = std::chrono::high_resolution_clock::now();
                auto currentTime = std::chrono::high_resolution_clock::now();
                float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

                glm::mat4 model = glm::mat4(1.0);
                glm::mat4 Projection = glm::perspective(glm::radians(45.f), 768.f/1280.f, 0.01f, 5000.f);
                glm::mat4 view = glm::lookAt(vec3(0,15,0),vec3(0,0,0),vec3(0,0,1));

                model = glm::rotate(model, time * glm::radians(12.f), glm::vec3(0.0f, 1.0f, 1.0f));
                //model = rotate(model, radians(90.f),vec3(1.0f,0.0f,0.0f));
                mat4 mvp = Projection * view * model;

                glUniformMatrix4fv(0,1,GL_FALSE,&mvp[0][0]);
            }
             
  
    }
    void update_mvp(EMesh* mesh){
       
       
                static auto startTime = std::chrono::high_resolution_clock::now();
                auto currentTime = std::chrono::high_resolution_clock::now();
                float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

                glm::mat4 model = glm::mat4(1.0);
                glm::mat4 Projection = glm::perspective(glm::radians(45.f), 768.f/1280.f, 0.01f, 5000.f);
                glm::mat4 view = glm::lookAt(vec3(0,15,0),vec3(0,0,0),vec3(0,0,1));

                model = glm::rotate(model, time * glm::radians(12.f), glm::vec3(0.0f, 1.0f, 1.0f));
                //model = rotate(model, radians(90.f),vec3(1.0f,0.0f,0.0f));
                mat4 mvp = Projection * view * mesh->model_matrix;

                glUniformMatrix4fv(0,1,GL_FALSE,&mesh->MVP[0][0]);
            
             
  
    }

    void draw_mesh(){
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glBindTexture(GL_TEXTURE_2D,textureid);
           // int samplerid = glGetUniformLocation(shaderProgram, "texture_sampler");
            //glUniform1i(samplerid, 0);
            
            
           // glDrawElements(GL_TRIANGLES,meshes[0]->indices.size(),GL_UNSIGNED_INT,(void*)0);

            glDrawArrays(GL_TRIANGLES,0,3);
            //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    }
#ifndef ANDROID
    void draw(EMesh* mesh){        
        glUseProgram  ( mesh->shader_program );
        glBindTexture(GL_TEXTURE_2D,mesh->texture_id);
        //update_mvp(mesh);
        glBindBuffer(GL_ARRAY_BUFFER,mesh->vertex_buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,mesh->indices_buffer);
        glDrawElements(GL_TRIANGLES,mesh->indices.size(),GL_UNSIGNED_INT,(void*)0);


    }
#endif
};

#endif


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
