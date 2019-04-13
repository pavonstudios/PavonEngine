#include "engine.h"


#include <sstream>
#ifndef ANDROID
	#include "Game/ThirdPerson.hpp"
	#include "input_controller.h"
#endif

#ifndef ANDROID
void Engine::main_loop(){
	 ThirdPerson player;
	 player.engine = this;
		player.mesh = meshes[3];
	 while (!glfwWindowShouldClose(window)) {
		 	print_fps();
		 	glfwPollEvents();
			update_input();
			if(!edit_mode)
				player.update();			
			get_time();
			main_camera.cameraSpeed = main_camera.velocity * deltaTime;

			
				auto tStart = std::chrono::high_resolution_clock::now();
				app.main_loop();//draw frame
				frames++;

				auto tEnd = std::chrono::high_resolution_clock::now();
				auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();
				frame_time = (float)tDiff/1000.0f;

				fps += (float)tDiff;
				if(fps > 1000.0f){
					last_fps = static_cast<uint32_t>((float)frames * (1000.0f / fps));
					fps = 0;
					frames = 0;
				}
			
		
			

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
		
	#ifdef VULKAN
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
			glfwSetWindowUserPointer(window, this);

	#ifdef VULKAN	   
			glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
	#endif
			glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
			glfwSetKeyCallback(window, input.key_callback);
			glfwSetCursorPosCallback(window, mouse_callback);
			glfwSetMouseButtonCallback(window,mouse_button_callback);
			glfwSetScrollCallback(window,input.scroll_callback);

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
	main_camera.screen_width = width;
	main_camera.screen_height = height;
	main_camera.update_projection_matrix();
}

 void Engine::mouse_callback(GLFWwindow* window, double xpos, double ypos){
	#ifdef VULKAN
	  auto app = reinterpret_cast<Renderer*>(glfwGetWindowUserPointer(window));
	#endif
	#ifdef _OpenGL_Renderer_
		auto app = reinterpret_cast<RendererGL*>(glfwGetWindowUserPointer(window));
	#endif

		if(!app){
			throw std::runtime_error("no app pointer");
		}
	
	if(app->engine->input.move_camera){
		if(app->engine->input.first_mouse){
			app->engine->input.lastX = xpos;
			app->engine->input.lastY = ypos;
			app->engine->input.first_mouse = false;
		}

		float xoffset = xpos - app->engine->input.lastX ;
		float yoffset = app->engine->input.lastY - ypos; // reversed since y-coordinates range from bottom to top
		app->engine->input.lastX = xpos;
		app->engine->input.lastY = ypos;

		float sensitivity = 0.05f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		app->engine->input.yaw   += xoffset;
		app->engine->input.pitch += yoffset;  

		if(app->engine->input.pitch > 89.0f)
			app->engine->input.pitch =  89.0f;
		if(app->engine->input.pitch < -89.0f)
			app->engine->input.pitch = -89.0f;		

	}//end right click pressed
	
	

}

void Engine::mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
	#ifdef VULKAN
		auto app = reinterpret_cast<Renderer*>(glfwGetWindowUserPointer(window));
	#endif
	#ifdef _OpenGL_Renderer_
		auto app = reinterpret_cast<RendererGL*>(glfwGetWindowUserPointer(window));
	#endif

		if (button == GLFW_MOUSE_BUTTON_RIGHT ){
			if(action == GLFW_PRESS){
				app->engine->input.right_button_pressed = true;
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);  
				app->engine->input.move_camera = false;
			}
			if(action == GLFW_RELEASE){
				app->engine->input.right_button_pressed = false;
			}

		}
		if (button == GLFW_MOUSE_BUTTON_LEFT ){
			if(action == GLFW_PRESS){
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
				app->engine->input.move_camera = true; 
			}
			if(action == GLFW_RELEASE){
				//app->engine->input.right_button_pressed = false;
			}

		}

}


void Engine::update_input(){
	
	if(edit_mode){
			if(input.S.bIsPressed){
				main_camera.MoveBackward();
				
			}
			if(input.W.bIsPressed){
				main_camera.MoveForward();
			}
			if(input.A.bIsPressed){		
				main_camera.MoveLeft();
			}
			if(input.D.bIsPressed){		
				main_camera.MoveRight();
				
			}
			if(input.Z.bIsPressed){
				meshes[1]->nodes[2]->move_root();
			}
			if(input.X.bIsPressed){	
				meshes[1]->nodes[2]->rotate();
			}

			if(input.Q.bIsPressed){
				main_camera.MoveDown();
			}
			if(input.E.bIsPressed){
				main_camera.MoveUp();
			}

			if(input.move_camera){
				main_camera.mouse_control_update(input.yaw, input.pitch);

			}

	}
}

void Engine::load_models(){	
	

}

void Engine::load_and_instance_at_location(std::string path, glm::vec3 location){
	#ifdef VULKAN
	EMesh *model = new EMesh(vulkan_device);	
	#else
	EMesh *model = new EMesh();
	#endif
	model->load_model_gltf(path.c_str());
	glm::mat4 model_matrix = glm::mat4(1.0f);
	model_matrix = glm::translate(model_matrix, location);
	model->model_matrix = model_matrix;
	meshes.push_back(model);
	//std::cout << "loading Emesh" << std::endl;

}
void Engine::Execute(){
			//pthread_create(&thread[0],NULL, ExecuteRenderHanler, this);
            //pthread_create(&thread[1],NULL, ExecuteInputHanler, this);
            InitWindow();
            Render();
}

void * Engine::Render(){
			#ifdef VULKAN
	        app.run(&vkdata);
			
			glfwSetScrollCallback(window,input.scroll_callback);
            load_map("map01.map");
            app.configure_objects();
            std::cout << "Rendering" << std::endl;
            main_loop();
            //pthread_exit(NULL);
			#endif
            return (void *)nullptr;
}
void Engine::load_map(std::string path){
	FILE* file = fopen(path.c_str(),"r");
	if(file == NULL){
		throw std::runtime_error("failed to load map file");

	}
	
	std::vector<std::string> models_paths;
	std::vector<glm::vec3> locations;
	std::vector<std::string> textures_paths;
	while(1){
		char lineHeader[128];
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
        	break; 

		if(!(strcmp( lineHeader, "#" ) == 0)){
			if ( strcmp( lineHeader, "m" ) == 0 ){
				char model_path[256];
				glm::vec3 location;
				char texture_path[256];
				fscanf(file, "%s %f %f %f %s\n", model_path, &location.x, &location.y, &location.z, texture_path);
				models_paths.push_back(std::string(model_path));
				locations.push_back(location);
				textures_paths.push_back(std::string(texture_path));
			}
			
		}
		
	}
	
	for(uint i = 0; i < models_paths.size();i++){		
		load_and_instance_at_location(models_paths[i],locations[i]);
				
	}

	//add textures path
	for(uint i = 0; i < models_paths.size();i++){	
	meshes[i]->texture_path = textures_paths[i];
	}
	//load gltf model
	//load_models();
}

void Engine::delete_meshes(){
	for(auto mesh : meshes){
		delete mesh;
	}
}
#ifdef VULKAN
void Engine::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
  auto app = reinterpret_cast<Renderer*>(glfwGetWindowUserPointer(window));
            app->framebufferResized = true;
}
#endif
void * Engine::InputHanled(){
	std::cout << "Input thread created" << std::endl;
            char character;
            bool repeat = true;
            while(repeat){
                if(!app.bIsRunnning){
                    std::cout << "Exiting from input console";
                    break;
                }
                std::cin >> character;
                if(character == 's'){
                    std::cout << "Letter S pressed";
                }
                if(character == 'f'){
                    repeat = false;
                }
                if(character == 'x'){
                    std::cout << "Rotation value changed to 45";
                    //app.RotationValue = 45;
                }
                if(character == 'z'){
                    std::cout << "LRotation Value changed to 90";
                     //app.RotationValue = 90;
                }
                if(character == 'm'){
                    std::cout << "Loading other model";
                    //app.loadModel("models/chalet.obj");
                }
                
            }
   
            pthread_exit(NULL);
}

#ifdef DEVELOPMENT
    void Engine::print_debug(const std::string text, int8_t posx, int8_t posy){		
		printf("%c[%i;%iH",0x1B,posx,posy);
		printf(text.c_str());		
	}
	void Engine::print_fps(){
		print_debug("",0,15);
		printf("FPS: ");
		printf("%i",last_fps);
		printf(" Frames: %i",frames);
		printf(" Frame time: %f",frame_time);
	}

#endif
float Engine::get_time(){
            static auto startTime = std::chrono::high_resolution_clock::now();

            auto currentTime = std::chrono::high_resolution_clock::now();
            float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

            deltaTime = time - lastFrame;
            lastFrame = time;


			
            return time;
}
#endif


#ifdef ANDROID
#include <android/log.h>
#include "android_helper.h"
	void Engine::create_window(android_app *pApp) {
		display = eglGetDisplay(EGL_DEFAULT_DISPLAY);


		eglInitialize(display, NULL, NULL);

		/* get an appropriate EGL frame buffer configuration */
		eglChooseConfig(display, attribute_list, &config, 1, &num_config);


		context = eglCreateContext(display, config, EGL_NO_CONTEXT, GiveMeGLES2);


		surface = eglCreateWindowSurface(display, config, pApp->window, NULL);

        eglMakeCurrent(display, surface, surface, context);

	}
#endif