#pragma once
#ifndef _Engine_H_
#define _Engine_H_

#define GLM_FORCE_SWIZZLE


#ifdef VULKAN
    #include "renderer.h"
    #define GLFW_INCLUDE_VULKAN
    #include "VulkanData.hpp"
    #include <GLFW/glfw3.h>
#endif    

#if defined (ES2)
    #include "renderers/opengl.hpp"
#endif

#include <chrono>
#include <vector>

#include "input.h"
#include "asset_manager.h"
#include "WindowManager.hpp"
#include "objects.h"
#include "camera.h"
#include "game_map.hpp"
#include "audio_manager.hpp"
class Game;

#include "gui.hpp"
#include "animation_manager.hpp"

#include  <sys/time.h>

class ConnectionManager;

using namespace engine;

class Engine {
  
public:
    Engine();
    Renderer renderer; 
    WindowManager window_manager;
     Camera main_camera;
      AssetManager assets;
    void draw_loading_screen();
     void update_mvp(EMesh* mesh);
    std::vector<EMesh*> meshes;//mesh to draw
    std::vector<EMesh*> linear_meshes;//loaded mesh 
     bool ready_to_game = false;
    bool loading = true;
    bool play_animations = false;
    Input input;
    bool edit_mode = false;
    std::vector<EMesh*> unique_meshes;
    TranslationUpdate tranlation_update;
    

    float deltaTime = 0.0f;	// Time between current frame and last frame

    bool draw_gizmos = false;

    
    
    std::vector<EMesh*> skeletal_meshes;//loaded mesh

    std::vector<EMesh*> helpers;
    std::vector<EMesh*> colliders_draw_mesh;  

    std::vector<Model*> models_to_draw;
    #ifdef ES2 
    std::vector<GLuint> texture_ids;
    #endif

    #ifdef ANDROID
        Engine(android_app * pApp);
        android_app * pAndroid_app;
    #endif
    #ifdef VULKAN        
        VulkanData vkdata = {VK_NULL_HANDLE};
        vks::VulkanDevice* vulkan_device = nullptr; 
        void vulkan_loop();     
        
        GLFWwindow* window;
  
    #endif    
    

    #ifdef LINUX
      
    Game * game = nullptr;
    void update_render_size();

   
    
    AudioManager audio_manager; 
    AnimationManager animation_manager; 
    
    
   
    
    MeshManager mesh_manager;
    TexturesManager textures_manager;
    
    MapManager maps;

    
    

    
    ConnectionManager* net_manager;

   
    
   
    
    
    

    float lastFrame = 0.0f; // Time of last frame

    int frames = 0;
    float fps = 0;
    int last_fps = 0;
    float frame_time = 0;
    struct timezone  tz;
	timeval  t1, t2;
    int  num_frames = 0;
    float LIMIT_FPS = 8;


    void configure_window_callback();
    void main_loop();
    void update_window_size();
    void delete_meshes();
    void init_renderer();
    void load_and_assing_location(std::string path, glm::vec3 location);
    void load_and_assing_location(struct MapDataToLoad data);
    void load_map(std::string path);
    float get_time();    
   
    void init();
    void es2_draw_frame();
    void loop_data();
   
    void distance_object_from_camera(); 

    void translate_mesh(EMesh* mesh, uint direction, float value);
 
    
    #ifdef DEVELOPMENT
        
        void print_debug(const std::string text, int8_t posx, int8_t posy);
        void print_fps();
        void calculate_time(std::string, std::chrono::time_point<std::chrono::system_clock>);
        void calculate_fps(std::chrono::time_point<std::chrono::system_clock>);
    #endif 

    void init_collision_engine();
    #endif   

    
};
#endif
