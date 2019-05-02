#include "engine.h"
#include <sstream>
#include <iostream>
#include <fstream>

#include "collision.h"

#include <thread>

#include "Game/game.hpp"
#include <unistd.h>

Engine::Engine(){

	#ifdef VULKAN 
		 renderer.engine = this;
	#endif

}

#ifdef ANDROID
	Engine::Engine(android_app * pApp){
        renderer.app = pApp;
        this->pAndroid_app = pApp;

	}
#endif

void Engine::draw_loading_screen(){
	#if defined(ES2)
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		GUI* loading = new GUI(this);
		EMesh* mesh = meshes[0];
		renderer.load_shaders(mesh);
		mesh_manager.create_buffers(mesh);
		renderer.load_mesh_texture(mesh);
		glUseProgram  ( mesh->shader_program );
		renderer.activate_vertex_attributes(mesh);
		update_mvp(mesh);
		renderer.draw_gui(mesh);
		window_manager.swap_buffers();
		meshes.clear();
		delete loading;
	#endif
}


void Engine::init(){
        window_manager.engine = this;
		#ifndef ANDROID
		window_manager.create_window();
        #endif
        #ifdef ANDROID
           window_manager.create_window(pAndroid_app);
        #endif


		maps.engine = this;

		draw_loading_screen();		
		
		configure_window_callback();

		#ifdef VULKAN
				
				renderer.run(&vkdata);		
	
				mesh_manager.vulkan_device = vulkan_device;
		#endif
		
		std::string map_path = assets.path("Maps/map01.map");

		game = new Game(this);
		load_map(map_path);
		game->init();

	auto tStart = std::chrono::high_resolution_clock::now();

		#ifdef VULKAN
			renderer.VulkanConfig();
		#endif

		mesh_manager.create_buffers(this,linear_meshes);

		#ifdef VULKAN			
			
			for(auto mesh : linear_meshes){
				renderer.load_mesh(mesh);
				renderer.update_descriptor_set(mesh);
			}			
			
			renderer.configure_objects();
				
		#endif


		#if defined(ES2) || defined(ANDROID)
			pipeline_data data = {};

			#ifndef ANDROID
				//data.fragment_shader_path = assets.path("frag.glsl");
				//data.vertex_shader_path = assets.path("vert_mvp.glsl");

				data.fragment_shader_path = "Game/Assets/frag.glsl";
				data.vertex_shader_path = "Game/Assets/vert_mvp.glsl";
			#endif

			#ifdef ANDROID
				data.fragment_shader_path = assets.path("shaders/gles/frag_sampler.glsl");
				data.vertex_shader_path = assets.path("vert_mvp.glsl");
			#endif		

			renderer.init_gl();   

			for(EMesh* mesh : linear_meshes){
				if(mesh->data.vertex_shader_path == ""){
						mesh->data = data;
				}
			}
			renderer.load_shaders(linear_meshes);
			renderer.load_textures(maps.same_textures);
			renderer.load_textures(linear_meshes);
			
			
				//edit_mode = true;
    	#endif

#ifdef DEVELOPMENT
	calculate_time(tStart);
#endif
		
		
		EMesh* mesh = meshes[3];

		std::thread col_thread(Collision::update_collision,mesh,game->player->mesh);
		col_thread.detach();
	
}


void Engine::loop_data(){
		#ifdef DEVELOPMENT
			print_fps();
		#endif			

		get_time();
		main_camera.cameraSpeed = main_camera.velocity * deltaTime;

		game->update();

		distance_object_from_camera();

		Objects::update_positions(this,tranlation_update);
		
}
void Engine::es2_loop() {

	#if defined(ES2) || defined(ANDROID)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		for(EMesh* mesh : meshes){
			
			glUseProgram  ( mesh->shader_program );
			renderer.activate_vertex_attributes(mesh);

			if(!mesh->bIsGUI){			
				
				update_mvp(mesh);
				renderer.draw(mesh);

			}					
			if(mesh->bIsGUI){
				
				renderer.update_mvp(mesh);
				renderer.draw_gui(mesh);

			}
		}
	#endif
}

void Engine::main_loop(){	
		
	while (!window_manager.window_should_close()) {
		window_manager.check_events();

		#ifndef ANDROID
			input.update_input(this);
		#endif

		loop_data();

		auto tStart = std::chrono::high_resolution_clock::now();

		#ifdef VULKAN
			renderer.draw_frame();			
		#endif
		
		#ifdef ES2
			es2_loop();
		#endif		

		frames++;
#ifdef DEVELOPMENT
		calculate_fps(tStart);
#endif
		window_manager.swap_buffers();

		tranlation_update.movements.clear();
		
		
	}
	
	delete game;
	#ifdef VULKAN
		renderer.finish();
		//window manager clear ?
	#endif
	

}
void Engine::update_render_size(){
	main_camera.update_projection_matrix();
   	#if defined (ES2) || (ANDROID)
   		glViewport(0,0,main_camera.screen_width,main_camera.screen_height);
   	#endif

    if(game){
        if(game->gui)
            game->gui->update_elements_mvp();
    }

}
void Engine::delete_meshes(){
	for(auto mesh : meshes){
		delete mesh;
	}
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

	void Engine::calculate_fps( std::chrono::time_point<std::chrono::system_clock> tStart){
	auto tEnd = std::chrono::high_resolution_clock::now();
	auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();
	frame_time = (float)tDiff/1000.0f;

	fps += (float)tDiff;
	if(fps > 1000.0f){
		last_fps = static_cast<uint32_t>((float)frames * (1000.0f / fps));
		fps = 0;
		frames = 0;
	}			

	}
	void Engine::calculate_time( std::chrono::time_point<std::chrono::system_clock> tStart){
		auto tEnd = std::chrono::high_resolution_clock::now();
		auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();
		float elapsed_time = (float)tDiff/1000.0f;	

		std::cout << "Loading mesh time: "	<< elapsed_time << std::endl;	

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

void Engine::update_mvp(EMesh* mesh){
	glm::mat4 mat = glm::mat4(1.0);
	if(mesh->bIsGUI){
		//TODO: 3d gui
		if(input.Z.bIsPressed){
			mat = translate(mat,vec3(-0.5,-0.5,0)) * scale(mat,vec3(0.1,0.1,1));
			mat = rotate(mat, radians(90.f),vec3(1,0,0));
			mat = rotate(mat, radians(90.f),vec3(0,0,1));
			mat = translate(mat,vec3(0,0,-100));
		
			mat = main_camera.Projection * main_camera.View * mat;
			
		}		
				
		//loading screen
		if(loading){
			mat = mat4(1.0);
			mat = rotate(mat,radians(180.f),vec3(1,0,0)) * scale(mat,vec3(0.3,0.3,1));
			loading = false;
			mesh->MVP = mat;
		}		
		
	}else{
		
		mat  = main_camera.Projection * main_camera.View * mesh->model_matrix;

	}

	if(!mesh->bIsGUI){
		mesh->MVP = mat;
	}
		
	#if defined(ES2) || defined(ANDROID)
	renderer.update_mvp(mesh);
	#endif

}

void Engine::configure_window_callback(){			
	#ifdef VULKAN	
		window = window_manager.get_window();
		glfwSetWindowUserPointer(window, this);
			
		glfwSetFramebufferSizeCallback(window, window_manager.framebufferResizeCallback);
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
		model->texture.format = VK_FORMAT_R8G8B8A8_UNORM;			
	#else
		EMesh *model = new EMesh();
    #endif
	#ifdef ANDROID
		mesh_manager.load_mode_gltf_android(model,path.c_str(),pAndroid_app->activity->assetManager);
	#else
		mesh_manager.load_model_gltf(model, path.c_str());
	#endif

	glm::mat4 model_matrix = glm::mat4(1.0f);
	model_matrix = glm::translate(model_matrix, location);
	model->location_vector = location;
	model->model_matrix = model_matrix;
	meshes.push_back(model);
	linear_meshes.push_back(model);
	
}

void Engine::load_and_assing_location(struct load_data data){
	std::string path = assets.path(data.model_path);
	vec3 location = data.location;

	#ifdef VULKAN
		EMesh *model = new EMesh(vulkan_device);//vulkan device for create vertex buffers
		model->texture.format = VK_FORMAT_R8G8B8A8_UNORM;			
	#else
		EMesh *model = new EMesh();
    #endif
	#ifdef ANDROID
		mesh_manager.load_mode_gltf_android(model,path.c_str(),pAndroid_app->activity->assetManager);
	#else
		mesh_manager.load_model_gltf(model, path.c_str());
	#endif

	model->name = data.model_path;

	glm::mat4 model_matrix = glm::mat4(1.0f);
	model_matrix = glm::translate(model_matrix, location);
	model->location_vector = location;
	model->model_matrix = model_matrix;
	if(data.type != MESH_LOD){
		meshes.push_back(model);
		
	}
	model->texture_path = assets.path(data.texture_path);
	linear_meshes.push_back(model);
}
//load objects paths

void Engine::load_map(std::string path){

    #ifndef ANDROID
	    
			std::stringstream file;
			std::ifstream text_file (path);
			if(text_file){
				file << text_file.rdbuf();
				text_file.close();
			}
			
    #endif

		#ifdef ANDROID
			AAsset* android_file = AAssetManager_open(pAndroid_app->activity->assetManager,path.c_str(), AASSET_MODE_BUFFER);

			size_t file_length = AAsset_getLength(android_file);
			char* fileContent = new char[file_length+1];

			AAsset_read(android_file, fileContent,file_length);
			AAsset_close(android_file);

			std::stringstream file ((std::string(fileContent)));

		#endif
			
		if(!file){
					#ifndef ANDROID
						throw std::runtime_error("failed to load map file");
					#endif
					#ifdef ANDROID
							LOGW("No file map");
					#endif
		}

		maps.load_data_from_file(file);


	for(auto mesh : meshes){		
		if(mesh->gltf_model.accessors[0].minValues.size() > 0){
		mesh->box.m_vecMax.x = mesh->gltf_model.accessors[0].maxValues[0];
		mesh->box.m_vecMax.y = mesh->gltf_model.accessors[0].maxValues[1];
		mesh->box.m_vecMax.z = mesh->gltf_model.accessors[0].maxValues[2];

		mesh->box.m_vecMin.x = mesh->gltf_model.accessors[0].minValues[0];
		mesh->box.m_vecMin.y = mesh->gltf_model.accessors[0].minValues[1];
		mesh->box.m_vecMin.z = mesh->gltf_model.accessors[0].minValues[2];

		}
	}
	
	#ifdef VULKAN
		pipeline_data data_static_mesh = {};
		
		data_static_mesh.draw_type = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		
		data_static_mesh.mesh_type = MESH_TYPE_STATIC;
		data_static_mesh.fragment_shader_path = "Game/Assets/shaders/frag.spv";
		data_static_mesh.vertex_shader_path = "Game/Assets/shaders/vert.spv";		

		for(EMesh* mesh : linear_meshes){
			mesh->data = data_static_mesh;
		}
		
	#endif


}

void Engine::translate_mesh(EMesh* mesh, uint direction, float value){
	
	
	vec3 direction_vector;
	switch (direction)
	{
		case FORWARD:
			direction_vector = vec3(0,-1,0);
			break;
		case BACKWARD:
			direction_vector = vec3(0,1,0);
			break;
		case LEFT:
			direction_vector = vec3(1,0,0);
			break;
		case RIGTH:
			direction_vector = vec3(-1,0,0);
			break;	
		
	}

	Movement movement = {direction_vector, value};	

	Collider collider = mesh->collider;
	if(collider.collision){
		if(collider.negative_x){
			collider.can_move_positive_x = false;
		}

		if(collider.positive_x){
			collider.can_move_negative_x = false;
		}
		if(collider.positive_y){
			collider.can_move_negative_y = false;
		}
		if(collider.negative_y){
			collider.can_move_positive_y = false;
		}
	}
	
	switch (direction)
	{
		case FORWARD:
			if(collider.can_move_positive_y){
			Objects::translate(this,mesh,movement);
			}
			break;
		case BACKWARD:
			if(collider.can_move_negative_y){
			Objects::translate(this,mesh,movement);
			}
			break;
		case LEFT:
			if(collider.can_move_negative_x){
				Objects::translate(this,mesh,movement);
			}
			break;
		case RIGTH:
			if(collider.can_move_positive_x){
				Objects::translate(this,mesh,movement);
			}
			break;	
		default:
			Objects::translate(this,mesh,movement);
			break;
		
	}

	
	
}

void Engine::distance_object_from_camera(){
	vec3 camera_position = main_camera.cameraPos;
	int mesh_id = 9;
	EMesh* mesh = linear_meshes[9];
	mesh->lod1 = linear_meshes[10];
	vec3 object_position = mesh->location_vector;

	bool erased = false;
	float distance;
	if(!erased){
		distance = glm::distance(camera_position,object_position);
	
		//std::cout << "distance: " << distance <<std::endl;
		if(distance > 15){
			meshes[9] = linear_meshes[11];
			erased = true;
			//std::cout << "erased" << std::endl;
		}
		if(distance < 15){
			meshes[9] = linear_meshes[10];
			erased = true;
			//std::cout << "erased" << std::endl;
		}
	}	
	
	
	
	
}