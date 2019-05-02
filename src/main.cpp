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
#include <android/input.h>
    extern "C" {

    int32_t handle_input(android_app* app, AInputEvent* event) {
        auto *pEngine = reinterpret_cast<Engine *>(app->userData);

        int32_t eventType = AInputEvent_getType(event);
        switch(eventType){
            case AINPUT_EVENT_TYPE_MOTION:
                switch(AInputEvent_getSource(event)){
                    case AINPUT_SOURCE_TOUCHSCREEN:
                        int action = AKeyEvent_getAction(event) & AMOTION_EVENT_ACTION_MASK;
                        switch(action){
                            case AMOTION_EVENT_ACTION_DOWN:
                                //float x =  AMotionEvent_getX(event,0);
                                //float y = AMotionEvent_getY(event,0);
                                //LOGW("touch in x = %f y = %f",x,y);
                                //pEngine->input.W.bIsPressed = true;
                                pEngine->input.input_event(event);
                                break;
                            case AMOTION_EVENT_ACTION_UP:
                                //pEngine->input.W.bIsPressed = false;
                                break;
                            case AMOTION_EVENT_ACTION_MOVE:
                                break;
                        }
                        break;
                } // end switch
                break;
            case AINPUT_EVENT_TYPE_KEY:
                // handle key input...
                break;
        } // end switch
        return  -1;
    }

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
        pApp->onInputEvent = handle_input;

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

                    pEngine->init();

                   pEngine->renderer.bReady = true;
                }
                if(pEngine->renderer.bReady){
                    //pEngine->renderer.render();
                    //pEngine->update_input();
                    pEngine->loop_data();
                    pEngine->es2_loop();
                    pEngine->window_manager.swap_buffers();

                }

            }



        } while (!pApp->destroyRequested);
    }
    }
#endif