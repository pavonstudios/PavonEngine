#include "engine.h"
#include <sstream>
#ifdef VULKAN
	#include "Game/ThirdPerson.hpp"	
#endif

#ifndef ANDROID
Engine::Engine(){
	#ifdef VULKAN 
		 app.engine = this;
	#endif
}
#endif

#ifdef ANDROID
	Engine::Engine(android_app * pApp){
        renderer.app = pApp;
        window_manager.create_window(pApp);
        renderer.init_gl();
	}
	Engine::Engine(){
			
	}
#endif
#ifdef VULKAN
void Engine::main_loop(){
	std::cout << "Rendering" << std::endl;

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




void Engine::init_renderer(){

      #ifdef VULKAN
				app.run(&vkdata);		
			//	load_map("Game/map01.map");//vulkan device must initialized before load object(it's becouse buffer need vulkan device for creation)	
			#endif
}



void Engine::delete_meshes(){
	for(auto mesh : meshes){
		delete mesh;
	}
}

void Engine::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
  auto app = reinterpret_cast<Renderer*>(glfwGetWindowUserPointer(window));
            app->framebufferResized = true;
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
#endif//end if def vulkan

#ifndef ANDROID
void Engine::update_mvp(EMesh* mesh){
	glm::mat4 mat = main_camera.Projection * main_camera.View * mesh->model_matrix;
	mesh->MVP = mat;
	#ifdef ES2
		renderer.update_mvp(mesh);
	#endif

}

void Engine::configure_window_callback(){			
	#ifdef VULKAN	
			window = window_manager.get_window();
			glfwSetWindowUserPointer(window, this);
			   
			glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
			glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
			glfwSetKeyCallback(window, input.key_callback);
			glfwSetCursorPosCallback(window, input.mouse_callback);
			glfwSetMouseButtonCallback(window,input.mouse_button_callback);
			glfwSetScrollCallback(window,input.scroll_callback);
	#endif			
}

void Engine::load_and_assing_location(std::string path, glm::vec3 location){
	#ifdef VULKAN
		EMesh *model = new EMesh(vulkan_device);//vulkan device for create vertex buffers	
	#else
		EMesh *model = new EMesh();
	#endif
	model->load_model_gltf(path.c_str());
	glm::mat4 model_matrix = glm::mat4(1.0f);
	model_matrix = glm::translate(model_matrix, location);
	model->model_matrix = model_matrix;
	meshes.push_back(model);	
}

void Engine::load_map(std::string path){
	//load objects paths
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
			if ( strcmp( lineHeader, "c" ) == 0 ){
				char model_path[256];
				glm::vec3 location;
				char texture_path[256];
				fscanf(file, "%s %f %f %f\n", model_path, &location.x, &location.y, &location.z);
				models_paths.push_back(std::string(model_path));
				locations.push_back(location);
				textures_paths.push_back(std::string("Game/Assets/textures/car01.jpg"));
			}
			
		}
		
	}
	
	for(uint i = 0; i < models_paths.size();i++){		
		load_and_assing_location(models_paths[i],locations[i]);				
	}

	//add textures path
	for(uint i = 0; i < models_paths.size();i++){	
	meshes[i]->texture_path = textures_paths[i];
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
#endif//if not define android
