

#ifndef ANDROID
    #ifdef VULKAN
        #include "engine.h"
    #endif
    int main() {
        #ifdef VULKAN
            Engine my_engine;

            try {

                my_engine.Execute();

            } catch (const std::exception& e) {

                std::cerr << e.what() << std::endl;
                
                return EXIT_FAILURE;
            }
            //pthread_exit(NULL);
            return EXIT_SUCCESS;
        #endif
    }
#endif

#ifdef ANDROID
    //#include "engine.h"
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