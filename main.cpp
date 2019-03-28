#include "engine.h"
int main(int argc, char *argv[]) {
    //Renderer app;

 /*    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    } */
    Engine my_engine;
    if(argc > 1){
        if(std::string(argv[1]) == "opengl"){
            std::cout << "opengl backend" << std::endl;
             my_engine.Execute(true);
        }
    }else{
        std::cout << "not input argument" << std::endl;
         my_engine.Execute(false);
    }
  
   //
    pthread_exit(NULL);
    return EXIT_SUCCESS;
}