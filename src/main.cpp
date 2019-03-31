
#ifdef ANDROID
#include "android_renderer.h"
#endif

#ifndef ANDROID
#include "engine.h"
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

#ifdef ANDROID
void android_main(struct android_app* state){

}
#endif