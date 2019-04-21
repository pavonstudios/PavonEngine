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
    #include "input_controller.h"
    #include "camera.h"
#else
#include "android_renderer.h"
    
#endif
#ifdef ES2
    
    #include "android_renderer.h"
#endif

#include <chrono>
#include <vector>
#include "asset_manager.h"
#include "WindowManager.hpp"
#include "3D_objects.h"
using namespace engine;
class Engine {
#ifdef ANDROID
public:
    Renderer renderer;
#endif
#ifdef ES2  
    
public:
    Renderer renderer;
    
#endif
public:
    WindowManager window_manager;
    AssetManager objects_manager;
    
    #ifdef ANDROID
        Engine(android_app * pApp);
        Engine();
    #endif

#ifndef ANDROID
    Engine();
    #ifdef VULKAN
        Renderer app;
        VulkanData vkdata = {VK_NULL_HANDLE};
        vks::VulkanDevice* vulkan_device;      
    #endif

        GLFWwindow* window;
        class Input input;

        std::vector<EMesh*> meshes;
        Camera main_camera;
        bool edit_mode = false;

        float deltaTime = 0.0f;	// Time between current frame and last frame
        float lastFrame = 0.0f; // Time of last frame  

        int frames = 0;
        float fps = 0;  
        int last_fps = 0;   
        float frame_time = 0;    
 
        GLFWwindow* get_window_pointer()
        {
            return window;
        }
        void InitWindow();
        void main_loop();
        void update_window_size();
        void delete_meshes();
        void Execute();    
        void load_and_assing_location(std::string path, glm::vec3 location);
        void load_map(std::string path);
        float get_time();
        void update_input();
        void update_mvp(EMesh* mesh);

        #ifdef DEVELOPMENT
        void print_debug(const std::string text, int8_t posx, int8_t posy);
        void print_fps();
        #endif 

    private:      
         
    #ifdef VULKAN
        static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
    #endif
        static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
        static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	       

#endif//end no define android

    


    
};
#endif