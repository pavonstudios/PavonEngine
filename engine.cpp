#include "engine.h"

void Engine::main_loop(){
	 while (!glfwWindowShouldClose(window)) {
		 	glfwPollEvents();
			 app.mainLoop();
    }
	app.finish();
	glfwDestroyWindow(window);

    glfwTerminate();


}
void Engine::InitWindow(){
	 	glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        window = glfwCreateWindow(WIDTH, HEIGHT, "Engine", nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
        glfwSetKeyCallback(window, key_callback);
        glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

}
void Engine::update_window_size(){
	 int width = 0, height = 0;
        while (width == 0 || height == 0) {
            glfwGetFramebufferSize(window, &width, &height);
            glfwWaitEvents();
        }

}
void Engine::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	  auto app = reinterpret_cast<Renderer*>(glfwGetWindowUserPointer(window));
            if (key == GLFW_KEY_E && action == GLFW_PRESS)
                app->RotationValue = 90;

            if (key == GLFW_KEY_W && action == GLFW_REPEAT){
                app->move_y += 1;
                app->main_camera.SetLocation(0,app->move_y,0);
            }
            
                
            if (key == GLFW_KEY_S && action == GLFW_REPEAT){
                app->move_y -= 1;
                app->main_camera.SetLocation(0,app->move_y,0);
            }
                
            if (key == GLFW_KEY_M && action == GLFW_REPEAT){
                //app->loadModel("models/chalet.obj");
                app->recreateSwapChain();
            }
}