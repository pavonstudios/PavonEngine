

#ifndef ANDROID
    #include "engine.h"
    #ifdef VULKAN
        int main() {
            
            Engine my_engine;

            try {

                my_engine.Execute();

            } catch (const std::exception& e) {

                std::cerr << e.what() << std::endl;
                
                return EXIT_FAILURE;
            }
            //pthread_exit(NULL);
            return EXIT_SUCCESS;      
        }
    #endif
    #ifdef ES2
        int main(){
            std::cout << "openg gl es2\n ";
            Engine engine;
            engine.window_manager.create_window();
            engine.renderer.init_gl();
           
            while(1){
                    glClearColor ( 0.4 , 0.06 , 0.07 , 1.);    // background color
                glClear ( GL_COLOR_BUFFER_BIT );
                engine.renderer.render();
                engine.window_manager.swap_buffers();
            }              

            return 1;
        }
    #endif
#endif

#ifdef ANDROID
    #include "engine.h"
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