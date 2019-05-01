#include "engine.h"
#include <sstream>
#include <iostream>
#include <fstream>


#include "Game/ThirdPerson.hpp"	
#include "Game/gui.hpp"
#include "Game/vehicle.hpp"

#include "collision.h"

#include <thread>


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
	#if defined(ES2) || defined(ANDROID)
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
void Engine::init_player(){

		//player = new ThirdPerson();
		//input.W.bIsPressed = false;
		player = new Vehicle();
		player->engine = this;	
		player->mesh = nullptr;
		player->camera_position = vec3(0,-2.5,-8.5);
		if(this->player_id == -1){
			std::runtime_error("no player assigned from map file");
		}else{
				
			player->mesh = this->meshes[this->player_id];
		}

		
		
		if(!player->mesh){
			std::runtime_error("no player mesh pointer assigner");
		}

}

void Engine::init(){
		#ifndef ANDROID
		window_manager.create_window();
		#endif		
		window_manager.engine = this;

		draw_loading_screen();
		
		pipeline_data data = {};

		#ifndef ANDROID
			data.fragment_shader_path = "android/app/src/main/assets/frag.glsl";
			data.vertex_shader_path = "android/app/src/main/assets/vert_mvp.glsl";
		#endif

		#ifdef ANDROID
			data.fragment_shader_path = "shaders/gles/frag_uv_color.glsl";
			data.vertex_shader_path = "vert_mvp.glsl";
    	#endif

		configure_window_callback();

		#ifdef VULKAN
				init_renderer();
				mesh_manager.vulkan_device = vulkan_device;
		#endif
		
		#ifdef ANDROID
			string map_path = "Maps/map01.map";
		#else
			std::string map_path = "Game/Assets/Maps/map01.map";
		#endif

		load_map(map_path);
		gui = new GUI(this);

	auto tStart = std::chrono::high_resolution_clock::now();

		mesh_manager.create_buffers(meshes);

		#ifdef VULKAN
			renderer.VulkanConfig();
			
			for(auto mesh : meshes){
				renderer.load_mesh(mesh);
				renderer.update_descriptor_set(mesh);
			}			
			
			renderer.configure_objects();
				
		#endif


		#if defined(ES2) || defined(ANDROID)
								
			renderer.init_gl();   

			for(EMesh* mesh : meshes){
				if(mesh->data.vertex_shader_path == ""){
						mesh->data = data;
				}
				renderer.load_shaders(mesh);
							
				#ifndef ANDROID
					renderer.load_mesh_texture(mesh);
				#endif
			}				

				edit_mode = true;
    	#endif

	calculate_time(tStart);

		init_player();

		EMesh* mesh = meshes[0];

		std::thread col_thread(Collision::update_collision,mesh,player->mesh);
		col_thread.detach();
	
}


void Engine::loop_data(){
		#ifdef DEVELOPMENT
			//print_fps();
		#endif

		if(!edit_mode){
			if(player->mesh){
					player->update();	
			}
			
		}
				

		get_time();
		main_camera.cameraSpeed = main_camera.velocity * deltaTime;

		Objects::update_positions(this,tranlation_update);

		if(input.left_button_pressed){
			gui->calculate_mouse_position();
		}
		
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
				update_mvp(mesh);
				renderer.draw_gui(mesh);

			}
		}
	#endif
}
#ifdef VULKAN
void Engine::vulkan_loop(){										
	renderer.main_loop();//draw frame
}
#endif

void Engine::main_loop(){	
	
		
	while (!window_manager.window_should_close()) {
		window_manager.check_events();

	#ifndef ANDROID
		input.update_input(this);
	#endif
		loop_data();

		auto tStart = std::chrono::high_resolution_clock::now();

		#ifdef VULKAN
			vulkan_loop();
		#endif
		
		#ifdef ES2
			es2_loop();
		#endif		

		frames++;
		
		calculate_fps(tStart);
		

		window_manager.swap_buffers();

		tranlation_update.movements.clear();
		tranlation_update.movements.clear();
		
	}
	delete gui;
	#ifdef VULKAN
		renderer.finish();
		glfwDestroyWindow(window);

		glfwTerminate();

	#endif//end if define vulkan

}


#ifdef VULKAN


void Engine::init_renderer(){

      #ifdef VULKAN
				renderer.run(&vkdata);		
			//	load_map("Game/map01.map");//vulkan device must initialized before load object(it's becouse buffer need vulkan device for creation)	
			#endif
}

void Engine::delete_meshes(){
	for(auto mesh : meshes){
		delete mesh;
	}
}

#endif//end if def vulkan

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
		}else{
			glm::mat4 projection = glm::ortho(0.0f, 1.0f*800, 1.0f*600, 0.0f);
			mat4 image_scale = glm::scale(mat,vec3(50,50,0));
			mat4 model_mat = translate(mat,vec3(400,300,0));
			model_mat = model_mat * image_scale;
			mat = projection * model_mat;
		}
		
			
	}else{
		
		mat  = main_camera.Projection * main_camera.View * mesh->model_matrix;

	}
	
	mesh->MVP = mat;

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
		model->load_mode_gltf_android(path.c_str(),pAndroid_app->activity->assetManager);
	#else
		mesh_manager.load_model_gltf(model, path.c_str());
	#endif

	glm::mat4 model_matrix = glm::mat4(1.0f);
	model_matrix = glm::translate(model_matrix, location);
	model->location_vector = location;
	model->model_matrix = model_matrix;
	meshes.push_back(model);
	
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


		std::vector<std::string> models_paths;
		std::vector<glm::vec3> locations;
		std::vector<std::string> textures_paths;
		std::string line;
		int counter = 0;
		std::vector<int> skeletal_id;
		int type = 0;
		while( std::getline(file,line) ) {		

			if(line != ""){
					char first_char;
				std::stringstream line_stream (line);
			
				std::string model_path;
				glm::vec3 location;
				std::string texture_path;
				std::string type;
				

				line_stream >> first_char >> model_path >> location.x >> location.y >> location.z >> texture_path >> type;				
				if(first_char == '/'){
					break;
				}
				if(first_char != '#'){

					if(first_char == 'c'){
						texture_path = "textures/car01.jpg";
					}
					if(first_char == 's'){
						skeletal_id.push_back(counter);
					}
					if(first_char == 'a'){
						//with collider
						type = MESH_WITH_COLLIDER;
					}
					models_paths.push_back(model_path);
					textures_paths.push_back(texture_path);
					locations.push_back(location);

					
					if(player_id == -1){
						if(type == "player"){
							this->player_id = counter;
							
						}
					}
					
					counter++;
					
				}			
			
			}		
			
		
		}

	#ifndef ANDROID
		//convert path to asset folder path
		std::vector<std::string> new_paths;
		for(std::string path : models_paths){
			std::string new_path = objects_manager.convert_to_asset_folder_path(path);
			new_paths.push_back(new_path);
		}
		models_paths = new_paths;

		new_paths.clear();
		for(std::string path : textures_paths){
			std::string new_path = objects_manager.convert_to_asset_folder_path(path);
			new_paths.push_back(new_path);
		}
		textures_paths = new_paths;
	#endif
	
	for(uint i = 0; i < models_paths.size();i++){		
		load_and_assing_location(models_paths[i],locations[i]);				
	}

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

	//add textures path
	for(uint i = 0; i < models_paths.size();i++){	
	meshes[i]->texture_path = textures_paths[i];
	}

	#ifdef VULKAN
	pipeline_data data_static_mesh = {};
	
    data_static_mesh.draw_type = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    
	data_static_mesh.mesh_type = MESH_TYPE_STATIC;
    data_static_mesh.fragment_shader_path = "Game/Assets/shaders/frag.spv";
    data_static_mesh.vertex_shader_path = "Game/Assets/shaders/vert.spv";


	pipeline_data data_skinned_mesh = {};
	
    data_skinned_mesh.draw_type = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	
    data_skinned_mesh.mesh_type = MESH_TYPE_SKINNED;
    data_skinned_mesh.fragment_shader_path = "Game/Assets/shaders/frag.spv";
    data_skinned_mesh.vertex_shader_path = "Game/Assets/shaders/skin.spv";

	for(EMesh* mesh : meshes){
		mesh->data = data_static_mesh;
	}
	for(int id : skeletal_id){//assing skinned shader
		meshes[id]->data = data_skinned_mesh;
		Skeletal::load_data(meshes[id]);
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