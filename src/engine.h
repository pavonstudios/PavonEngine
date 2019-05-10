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

#if defined (ES2) || defined (ANDROID)  
    #include "android_renderer.h"
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
#include "Game/gui.hpp"
#include "animation_manager.hpp"

#include  <sys/time.h>

using namespace engine;

class Engine {
  
public:
    Engine();

    #ifdef ANDROID
        Engine(android_app * pApp);
        android_app * pAndroid_app;
    #endif

    Renderer renderer;   
    WindowManager window_manager;
    AssetManager assets;
    Camera main_camera;
    MeshManager mesh_manager;
    Input input;
    MapManager maps;
#ifndef  ANDROID
    AudioManager audio_manager;
#endif
    TranslationUpdate tranlation_update;
    AnimationManager animation_manager;

    Game * game;

   
    bool ready_to_game = false;
    bool loading = true;
    bool play_animations = false;
    float animation_time = 0.0f;   
   

    std::vector<EMesh*> meshes;//mesh to draw
    std::vector<EMesh*> linear_meshes;//loaded mesh 
    std::vector<EMesh*> unique_meshes;
    std::vector<EMesh*> skeletal_meshes;//loaded mesh

    std::vector<EMesh*> helpers;
    std::vector<EMesh*> colliders_draw_mesh;  

    std::vector<Model*> models_to_draw;
    #ifdef ES2 
    std::vector<GLuint> texture_ids;
    #endif
    
    
    bool edit_mode = false;

    float deltaTime = 0.0f;	// Time between current frame and last frame
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
    void update_mvp(EMesh* mesh);
    void init();
    void es2_draw_frame();
    void loop_data();
    void update_render_size();
    void distance_object_from_camera(); 

    void translate_mesh(EMesh* mesh, uint direction, float value);
 
    void draw_loading_screen();
    #ifdef DEVELOPMENT
        bool draw_gizmos = false;
        void print_debug(const std::string text, int8_t posx, int8_t posy);
        void print_fps();
        void calculate_time(std::chrono::time_point<std::chrono::system_clock>);
        void calculate_fps(std::chrono::time_point<std::chrono::system_clock>);
    #endif 

    void init_collision_engine();

    
#ifdef VULKAN        
    VulkanData vkdata = {VK_NULL_HANDLE};
    vks::VulkanDevice* vulkan_device; 
    void vulkan_loop();     
    
    GLFWwindow* window;
    GLFWwindow* get_window_pointer()
    {
        return window;
    }    
#endif
    




    


    
};
#endif
