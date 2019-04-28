#pragma once
#ifndef _Engine_H_
#define _Engine_H_

#ifndef ANDROID
    #ifdef VULKAN
        #include "renderer.h"
        #define GLFW_INCLUDE_VULKAN
        #include "VulkanData.hpp"
    #endif
    #include <GLFW/glfw3.h>
    #include <pthread.h>

#else
    #include "android_renderer.h"
#endif

#ifdef ES2    
    #include "android_renderer.h"
#endif

#include <chrono>
#include <vector>

#include "input.h"
#include "asset_manager.h"
#include "WindowManager.hpp"
#include "objects.h"
#include "camera.h"

class ThirdPerson;
class GUI;

using namespace engine;

class Engine {
  
public:
    Renderer renderer; 

    Engine();
    WindowManager window_manager;
    AssetManager objects_manager;
    Camera main_camera;
    MeshManager mesh_manager;
    Input input;

    ThirdPerson * player;
    GUI* gui;
    
    #ifdef ANDROID
        Engine(android_app * pApp);
        android_app * pAndroid_app;
    #endif

    std::vector<EMesh*> meshes;
    int player_id = -1;
    
    bool edit_mode = false;

    float deltaTime = 0.0f;	// Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame

    int frames = 0;
    float fps = 0;
    int last_fps = 0;
    float frame_time = 0;

    void configure_window_callback();
    void main_loop();
    void update_window_size();
    void delete_meshes();
    void init_renderer();
    void load_and_assing_location(std::string path, glm::vec3 location);
    void load_map(std::string path);
    float get_time();
    void update_input();
    void update_mvp(EMesh* mesh);
    void init();
    void es2_loop();
    void loop_data();
    void init_player();    
    
    #ifdef DEVELOPMENT
        void print_debug(const std::string text, int8_t posx, int8_t posy);
        void print_fps();
        void calculate_time(std::chrono::time_point<std::chrono::system_clock>);
        void calculate_fps(std::chrono::time_point<std::chrono::system_clock>);
    #endif 

#ifndef ANDROID
    
    #ifdef VULKAN        
        VulkanData vkdata = {VK_NULL_HANDLE};
        vks::VulkanDevice* vulkan_device; 
        void vulkan_loop();     
    #endif

        GLFWwindow* window;
        GLFWwindow* get_window_pointer()
        {
            return window;
        }   
    void load_meshes_async();
    void * load_meshes(void* c);
     void * load_meshes01(void* c);
    static void * load_helper(void * context);
    static void * load_helper1(void * context);
    int loading_mesh01 = false;
    int loading1 = false;
    int loading2 = false;

    static void update_collision(EMesh* mesh, EMesh* mesh2);

private:
    std::vector<EMesh*> meshes_01;
	std::vector<EMesh*> meshes_02;

#endif//end no define android

    


    
};
#endif