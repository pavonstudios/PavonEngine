
#include "engine.h"
#ifndef ANDROID
    
   
        int main() {
            
            Engine engine;
            engine.window_manager.create_window();
            pipeline_data data = {};
                data.fragment_shader_path = "android/app/src/main/assets/frag.glsl";
                data.vertex_shader_path = "android/app/src/main/assets/vert_mvp.glsl";
                
            #ifdef ES2
                std::cout << "openg gl es2\n ";
                
                engine.load_map("map01.map");
                int id_object =0  ;
                int id_secode = 4;
                engine.renderer.init_gl();
                
                
                    engine.meshes[id_object]->data = data;
                    engine.renderer.load_shaders(engine.meshes[id_object]);
                    engine.meshes[0]->create_buffers();
                    engine.renderer.load_mesh_texture(engine.meshes[id_object]);
                    id_object = 4;
                     engine.meshes[id_object]->data = data;
                    engine.renderer.load_shaders(engine.meshes[id_object]);
                    engine.meshes[4]->create_buffers();
                    engine.renderer.load_mesh_texture(engine.meshes[id_object]);
                    
                    id_object = 0;
                    while(1){
                        engine.renderer.activate_vertex_attributes(engine.meshes[0]);
                        engine.renderer.draw(engine.meshes[0]);
                        
                      engine.renderer.activate_vertex_attributes(engine.meshes[4]);
                        engine.renderer.draw(engine.meshes[4]);
                        
                        
                        engine.window_manager.swap_buffers();
                    }              
            #endif
            #ifdef VULKAN
                engine.Execute();
            #endif

            //pthread_exit(NULL);
            return EXIT_SUCCESS;      
        }
   
   
#endif

#ifdef ANDROID
    #include "engine.h"

    extern "C" {
    void handle_cmd(android_app *pApp, int32_t cmd) {
        switch (cmd) {
            case APP_CMD_INIT_WINDOW:
                    
                    pApp->userData = new Engine(pApp);
                                        
                    break;




            case APP_CMD_TERM_WINDOW:
                if (pApp->userData) {
                    auto *pEngine = reinterpret_cast<Engine *>(pApp->userData);
                    pApp->userData = nullptr;
                    delete pEngine;
                }
        }
    }

    void android_main(struct android_app *pApp) {
        pApp->onAppCmd = handle_cmd;
        pApp->userData;

        int events;
        android_poll_source *pSource;
        do {
            if (ALooper_pollAll(0, nullptr, &events, (void **) &pSource) >= 0) {
                if (pSource) {
                    pSource->process(pApp, pSource);
                }
            }
            if (pApp->userData) {
                auto *pEngine = reinterpret_cast<Engine *>(pApp->userData);
                pEngine->renderer.render();
                pEngine->window_manager.swap_buffers();
            }



        } while (!pApp->destroyRequested);
    }
    }
#endif