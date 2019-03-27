#include "engine.h"
int main() {
    //Renderer app;

 /*    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    } */
    Engine my_engine;
    my_engine.Execute();
    pthread_exit(NULL);
    return EXIT_SUCCESS;
}