#ifndef OPENGL_HPP
#define OPENGL_HPP

#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>

#include <errno.h>
#include <cassert>
#include <vector>

#ifdef WINDOWS
    #include <GL/gl.h>
    //#include <GL/glew.h>
#endif
#ifdef LINUX
    #include <GLES2/gl2.h>
#endif

#ifdef ANDROID
    #include <android/log.h>
    #include <android_native_app_glue.h>
    #include <jni.h>
    #include "../glm/glm.hpp"
    #include "../glm/gtc/matrix_transform.hpp"
    #include "../android_helper.h"
#endif

#include "../objects.h"
#include "../asset_manager.h"

using  namespace engine;
using namespace std;
using namespace glm;

struct shader_src{
    std::string vertex_shader_path;
    std::string fragment_shader_path;
    const char* vertex_code;
    const char* fragment_code;
    int type = -1;
};


class Renderer{
public:
    bool bReady = false;
    EMesh* triangle;
	GLuint vertex_buffer;
	std::vector<GLuint> textures_ids;
#ifdef ANDROID
    struct android_app * app;
#endif

private:
   
    GLuint indices;

    GLuint textureid;


    GLuint shaderProgram;

    GLuint load_shader (shader_src &shader_data, GLenum type);         

    void print_shader_info_log (GLuint shader, shader_src &shader_data, const char* src);
    
    char* load_shader_file(const char* path);
     
public:

    void load_shaders(EMesh* mesh);

    void load_shaders(const std::vector<EMesh*>& meshes); 

    void init_gl();   
	
   void load_mesh_texture(EMesh* mesh);      
    
    void load_textures(const std::vector<EMesh*>& meshes);      

    void load_textures(std::vector<std::string> &textures_paths);     
        
    void activate_vertex_attributes(EMesh* mesh);   
   
    void update_mvp(EMesh* mesh);                                   

    void draw_mesh();               

    void draw_gui(EMesh* mesh);

    void draw(EMesh* mesh);       
};

#endif