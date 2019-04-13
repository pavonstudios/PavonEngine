#pragma once
#ifndef _Engine_H_
#define _Engine_H_

#ifndef ANDROID
    #include "asset_manager.h"
    #ifdef VULKAN
        #include "renderer.h"
        #define GLFW_INCLUDE_VULKAN
        #include "VulkanData.hpp"
    #endif

    #ifdef _OpenGL_Renderer_
        #include "opengl_renderer.h"
    #endif
    #include <pthread.h>
    #include <GLFW/glfw3.h>
    #include "input_controller.h"
#else
   // #include "opengl_renderer.h"
   #include <EGL/egl.h>
   #include <android_native_app_glue.h>

#endif

#include <chrono>
#include <vector>

#include "3D_objects.h"
class Engine {
public:



#ifndef ANDROID
    #ifdef _OpenGL_Renderer_
        RendererGL app;
       // class Renderer renderer;
    #endif
    #ifdef VULKAN
        Renderer app;
        VulkanData vkdata = {VK_NULL_HANDLE};
        VkDevice * pDevice;
        vks::VulkanDevice* vulkan_device;
    #endif
        pthread_t thread[2];
        GLFWwindow* window;
        class Input input;
        AssetManager objects_manager;
        float move_y = 0;
        std::vector<EMesh*> meshes;
        EMesh* skeletal;
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
        void *Render();
        void load_models();
        void load_and_instance_at_location(std::string path, glm::vec3 location);
        void load_map(std::string path);
        float get_time();
        void update_input();

        #ifdef DEVELOPMENT
        void print_debug(const std::string text, int8_t posx, int8_t posy);
        void print_fps();
        #endif
        


        Engine(){init();};
        void init(){
            app.engine = this;          
            
            };
    private:
       
         
    #ifdef VULKAN
        static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
    #endif
        static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
        static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	
        static void* ExecuteInputHanler(void* This){
            return ((Engine *)This)->InputHanled();
        }
        static void* ExecuteRenderHanler(void * This){
            return ((Engine *)This)->Render();
        }
        void *InputHanled();

#endif//end no define android
     
#ifdef ANDROID
    EGLDisplay display;
    EGLConfig config;
    EGLContext context;
    EGLSurface surface;
    EGLint num_config;
    void create_window(android_app *pApp);
#endif
    
};
#endif