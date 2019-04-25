#define STB_IMAGE_IMPLEMENTATION
#include "engine.h"

#ifndef ANDROID
    
   
        int main() {
            
            Engine engine;
            engine.init();      
            engine.main_loop();                  

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
                //init gl and window
                if(!pEngine->renderer.bReady){
                   pEngine->window_manager.create_window(pApp);
                   pEngine->init();
                   pEngine->renderer.bReady = true;
                }
                if(pEngine->renderer.bReady){
                    //pEngine->renderer.render();
                    pEngine->loop_data();
                    pEngine->es2_loop();
                    pEngine->window_manager.swap_buffers();

                }

            }



        } while (!pApp->destroyRequested);
    }
    }
#endif