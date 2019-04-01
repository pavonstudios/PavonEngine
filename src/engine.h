#ifndef _Engine_H_
#define _Engine_H_

#ifndef ANDROID
#ifndef _OpenGL_Renderer_
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
#ifndef _OpenGL_Renderer_
        Renderer app;
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
            init();
            InitWindow();
            Render();

            }


        void *Render(){
            std::cout << "Rendering" << std::endl;

            app.run();
            main_loop();

            //pthread_exit(NULL);
        }
        void main_loop();
        
private:
         static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
#ifndef _OpenGL_Renderer_
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


        float get_time(){
            static auto startTime = std::chrono::high_resolution_clock::now();

            auto currentTime = std::chrono::high_resolution_clock::now();
            float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
            return time;
        }
        Engine(){init();};
        void init(){app.engine = this;};

    
};
#endif