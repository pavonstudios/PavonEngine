#include "engine.h"
#include "input_controller.h"

void Engine::main_loop(){
	 
	 while (!glfwWindowShouldClose(window)) {
		 	 glfwPollEvents();
			 update_input();
			   
			 app.main_loop();
			glfwSwapBuffers(window);
    }
	app.finish();
	glfwDestroyWindow(window);

    glfwTerminate();


}
void Engine::InitWindow(){
	move_y = 2;
		if( !glfwInit() )
		{
			fprintf( stderr, "Failed to initialize GLFW\n" );
			return;
		}
		
#ifndef _OpenGL_Renderer_
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        

#endif
#ifdef _OpenGL_Renderer_
		glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 
		
#endif

	
	window = glfwCreateWindow(800, 600, "Engine", nullptr, nullptr);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window\n" );
		glfwTerminate();
		return;
	}
#ifndef _OpenGL_Renderer_
	    glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
#endif
        glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
		glfwSetKeyCallback(window, key_callback);
#ifdef _OpenGL_Renderer_
	glfwMakeContextCurrent(window); // Initialize GLEW
	

		if (glewInit() != GLEW_OK) {
			fprintf(stderr, "Failed to initialize GLEW\n");
			return;
		}
#endif
}
void Engine::update_window_size(){
	 int width = 0, height = 0;
        while (width == 0 || height == 0) {
            glfwGetFramebufferSize(window, &width, &height);
            glfwWaitEvents();
        }

}
void Engine::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
#ifndef _OpenGL_Renderer_
	  auto app = reinterpret_cast<Renderer*>(glfwGetWindowUserPointer(window));
#endif
#ifdef _OpenGL_Renderer_
	  auto app = reinterpret_cast<RendererGL*>(glfwGetWindowUserPointer(window));
#endif
          
			if(key == GLFW_KEY_D){
				if(action == GLFW_PRESS){
					app->engine->input.D.bIsPressed = true;
				}
				if(action == GLFW_RELEASE){
					app->engine->input.D.bIsPressed = false;
				}
			}
			if(key == GLFW_KEY_A){
				if(action == GLFW_PRESS){
					app->engine->input.A.bIsPressed = true;
				}
				if(action == GLFW_RELEASE){
					app->engine->input.A.bIsPressed = false;
				}
			}
           
             if (key == GLFW_KEY_W && action == GLFW_PRESS) {
				app->engine->input.bIsKeyW_pressed = true;
              
            }
			else if (key == GLFW_KEY_W && action == GLFW_RELEASE)
			{
				app->engine->input.bIsKeyW_pressed = false;
			}
                
            if (key == GLFW_KEY_S && action == GLFW_PRESS) {
				app->engine->input.bIsKeyS_pressed = true;
              
            }
			else if (key == GLFW_KEY_S && action == GLFW_RELEASE)
			{
				app->engine->input.bIsKeyS_pressed = false;
			}
			
                
            if (key == GLFW_KEY_M && action == GLFW_REPEAT){
                //app->loadModel("models/chalet.obj");
                //app->recreateSwapChain();
            }
}

void Engine::update_input(){
	if(input.bIsKeyS_pressed){
		move_y += 0.001f;
		app.main_camera.SetLocation(0,move_y,0);
	}
	if(input.bIsKeyW_pressed){
		move_y -= 0.001f;
		app.main_camera.SetLocation(0,move_y,0);
	}
	if(input.A.bIsPressed){
		move_y += 0.001f;
		app.main_camera.SetLocation(move_y,0,0);
	}
	if(input.D.bIsPressed){
		move_y -= 0.001f;
		app.main_camera.SetLocation(move_y,0,0);
	}
}