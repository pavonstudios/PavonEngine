
#include "engine.h"
#ifndef ANDROID
    
   
        int main() {
            
            Engine engine;
            #ifdef ES2
                std::cout << "openg gl es2\n ";
                engine.window_manager.create_window();
                engine.load_map("map01.map");
                engine.renderer.init_gl();
                engine.meshes[0]->create_buffers();
                engine.renderer.activate_vertex_attributes();
                engine.renderer.create_texture();
                    while(1){
                        engine.renderer.render();
                        engine.renderer.draw(engine.meshes[0]);
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
    
    #include "android_renderer.h"
    extern "C" {
    void handle_cmd(android_app *pApp, int32_t cmd) {
        switch (cmd) {
            case APP_CMD_INIT_WINDOW:

                    //pApp->userData = new Engine();
                    pApp->userData = new Renderer(pApp);
                    //Renderer renderer = Renderer(pApp);
                    
                    break;




            case APP_CMD_TERM_WINDOW:
                if (pApp->userData) {
                // auto *pRenderer = reinterpret_cast<Engine *>(pApp->userData);
                    auto *pRenderer = reinterpret_cast<Renderer *>(pApp->userData);
                    pApp->userData = nullptr;
                    delete pRenderer;
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
                //auto *pRenderer = reinterpret_cast<Engine *>(pApp->userData);
                auto *pRenderer = reinterpret_cast<Renderer *>(pApp->userData);
                pRenderer->render();
            }



        } while (!pApp->destroyRequested);
    }
    }
#endif