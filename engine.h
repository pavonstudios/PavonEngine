#ifndef _Engine_H_
#define _Engine_H_

#include <pthread.h>

#ifndef _OpenGL_Renderer_
#include "renderer.h"
#endif

#ifdef _OpenGL_Renderer_
#include "opengl_renderer.h"
#endif
/* Engine class who controll 
threads executions */
class Engine {
    public:
#ifndef _OpenGL_Renderer_
        Renderer app;
#endif
#ifdef _OpenGL_Renderer_
        RendererGL app;
#endif
        pthread_t thread[2];
        void Execute(){            
            
            pthread_create(&thread[0],NULL, ExecuteRenderHanler, this);
            
            pthread_create(&thread[1],NULL, ExecuteInputHanler, this);
          
          
            
            }
    private:
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

        void *Render(){
            std::cout << "Rendering" << std::endl;

            app.run();

            
            pthread_exit(NULL);
        }

};
#endif