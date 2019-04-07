#pragma once
#ifndef _Engine_H_
#define _Engine_H_

#ifndef ANDROID
    #include "asset_manager.h"
    #ifdef VULKAN
        #include "renderer.h"
        #define GLFW_INCLUDE_VULKAN
    #endif

    #ifdef _OpenGL_Renderer_
        #include "opengl_renderer.h"
    #endif
    #include <pthread.h>
    #include <GLFW/glfw3.h>
#else
    #include "opengl_renderer.h"
#endif

#include <chrono>
#include "input_controller.h"




/* Engine class who controll 
threads executions */

class Engine {
    public:

#ifdef _OpenGL_Renderer_
        RendererGL app;
       // class Renderer renderer;
#endif
#ifndef ANDROID
    #ifdef VULKAN
        Renderer app;
        VkDevice* pDevice;
    #endif
        pthread_t thread[2];
        GLFWwindow* window;
        GLFWwindow* get_window_pointer()
        {
            return window;
        }
        void InitWindow();
      
        void update_window_size();
         void Execute(){

            //pthread_create(&thread[0],NULL, ExecuteRenderHanler, this);

            //pthread_create(&thread[1],NULL, ExecuteInputHanler, this);
            //init();
            InitWindow();
            Render();

            }


        void *Render(){
            

            app.run();
            load_map("map01.map");
            app.configure_objects();
            std::cout << "Rendering" << std::endl;
            main_loop();

            //pthread_exit(NULL);
            return (void *)nullptr;
        }
        void main_loop();
        
private:
         static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
         static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
         static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
         
    #ifdef VULKAN
        static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
            auto app = reinterpret_cast<Renderer*>(glfwGetWindowUserPointer(window));
            app->framebufferResized = true;
        }
    #endif
	
        static void* ExecuteInputHanler(void* This){
            return ((Engine *)This)->InputHanled();
        }
        static void* ExecuteRenderHanler(void * This){
            return ((Engine *)This)->Render();
        }
        void *InputHanled(){
    
            std::cout << "Input thread created" << std::endl;
            char character;
            bool repeat = true;
            while(repeat){
                if(!app.bIsRunnning){
                    std::cout << "Exiting from input console";
                    break;
                }
                std::cin >> character;
                if(character == 's'){
                    std::cout << "Letter S pressed";
                }
                if(character == 'f'){
                    repeat = false;
                }
                if(character == 'x'){
                    std::cout << "Rotation value changed to 45";
                    //app.RotationValue = 45;
                }
                if(character == 'z'){
                    std::cout << "LRotation Value changed to 90";
                     //app.RotationValue = 90;
                }
                if(character == 'm'){
                    std::cout << "Loading other model";
                    //app.loadModel("models/chalet.obj");
                }
                
            }
   
            pthread_exit(NULL);
        }

#endif
    void update_input();
public:
        class Input input;
        AssetManager objects_manager;
        float move_y = 0;
        std::vector<EMesh*> meshes;
        EMesh* skeletal;

        Camera main_camera; 
        glm::vec3 objects_positions[10] = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
        };    

        void load_models();
        void load_and_instance_at_location(std::string path, glm::vec3 location);
        void load_map(std::string path);

        float deltaTime = 0.0f;	// Time between current frame and last frame
        float lastFrame = 0.0f; // Time of last frame

        

        float get_time(){
            static auto startTime = std::chrono::high_resolution_clock::now();

            auto currentTime = std::chrono::high_resolution_clock::now();
            float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

            deltaTime = time - lastFrame;
            lastFrame = time;

            return time;
        }
        Engine(){init();};
        void init(){
            app.engine = this;
            
            
            };
        
    
};
#endif