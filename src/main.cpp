
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
                
            
                for(EMesh* mesh : engine.meshes){
                    mesh->data = data;
                    engine.renderer.load_shaders(mesh);
                    mesh->create_buffers();
                    engine.renderer.load_mesh_texture(mesh);
                }
                  
                    while(1){
                        engine.window_manager.check_events();
                        for(EMesh* mesh : engine.meshes){
                            engine.renderer.activate_vertex_attributes(mesh);
                            engine.renderer.draw(mesh);
                        }
                      
                        
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