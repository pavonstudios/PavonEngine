#include "engine.h"
#include <sstream>
#include <iostream>
#include <fstream>


#include "Game/ThirdPerson.hpp"	
#include "Game/gui.hpp"


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
void Engine::init_player(){

		player = new ThirdPerson();
		//input.W.bIsPressed = false;
		player->engine = this;	
		player->mesh = nullptr;
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
		#endif

		#ifdef ANDROID
			string map_path = "Maps/map01.map";
		#else
			std::string map_path = "Game/Assets/Maps/map01.map";
		#endif

		load_map(map_path);
		gui = new GUI(this);

		#ifdef VULKAN
				renderer.configure_objects();
		#endif


		#if defined(ES2) || defined(ANDROID)
								
				renderer.init_gl();   


                for(EMesh* mesh : meshes){
                                if(mesh->data.vertex_shader_path == ""){
                                        mesh->data = data;
                                }
                                renderer.load_shaders(mesh);
                                mesh_manager.create_buffers(mesh);
                        #ifndef ANDROID
                                renderer.load_mesh_texture(mesh);
                        #endif
                }

				//edit_mode = true;
    #endif


				init_player();
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
		
}
void Engine::es2_loop() {

		#if defined(ES2) || defined(ANDROID)
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		#endif

		#if defined(ES2) || defined(ANDROID)

			for(EMesh* mesh : meshes){
				
				if(!mesh->bIsGUI){
					glUseProgram  ( mesh->shader_program );
					renderer.activate_vertex_attributes(mesh);
					update_mvp(mesh);
					renderer.draw(mesh);
				}					
				if(mesh->bIsGUI){
					glUseProgram  ( mesh->shader_program );
					renderer.activate_vertex_attributes(mesh);
					renderer.draw_gui(mesh);
				}

			}

		/* 	glUseProgram(meshes.back()->shader_program);
			glBindBuffer(GL_ARRAY_BUFFER,renderer.vertex_buffer);
			glVertexAttribPointer ( 0, 3, GL_FLOAT, false, sizeof(Vertex), (void*)0 );
      glEnableVertexAttribArray ( 0 );        
			renderer.draw_mesh(); */


    #endif
    #ifdef ANDROID
       /*  glUseProgram  ( meshes[0]->shader_program );
        renderer.activate_vertex_attributes(meshes[0]);
        update_mvp(meshes[0]);
        renderer.draw(meshes[0]); */
        /* renderer.activate_vertex_attributes(meshes[4]);
        update_mvp(meshes[4]);
        renderer.draw(meshes[4]); */

        /* for(EMesh* mesh : meshes){
            renderer.activate_vertex_attributes(mesh);
            update_mvp(mesh);
            renderer.draw(mesh);
        } */
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
					update_input();
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

					auto tEnd = std::chrono::high_resolution_clock::now();
					auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();
					frame_time = (float)tDiff/1000.0f;

					fps += (float)tDiff;
					if(fps > 1000.0f){
						last_fps = static_cast<uint32_t>((float)frames * (1000.0f / fps));
						fps = 0;
						frames = 0;
					}			


					window_manager.swap_buffers();
					
			}

			#ifdef VULKAN
				renderer.finish();
				glfwDestroyWindow(window);

				glfwTerminate();

			#endif//end if define vulkan
	
}


#ifdef VULKAN
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
			glm::mat4 Projection = glm::ortho(0.0f, (float)800,(float)600,0.0f, 0.1f, 100.0f);
			glm::mat4 model_view_inverse = mesh->model_matrix * glm::inverse(main_camera.View);
			//mesh->model_matrix = model_view_inverse;
			//mat = Projection * main_camera.View * mesh->model_matrix;

		}else{
				mat  = main_camera.Projection * main_camera.View * mesh->model_matrix;

		}
		mat  = main_camera.Projection * main_camera.View * mesh->model_matrix;
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
		while( std::getline(file,line) ) {		

			if(line != ""){
					char first_char;
				std::stringstream line_stream (line);
			
				std::string model_path;
				glm::vec3 location;
				std::string texture_path;
				std::string type;
				

				line_stream >> first_char >> model_path >> location.x >> location.y >> location.z >> texture_path >> type;				
				
				if(first_char != '#'){

					if(first_char == 'c'){
						texture_path = "textures/car01.jpg";
					}
					if(first_char == 's'){
						skeletal_id.push_back(counter);
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

	

	//add textures path
	for(uint i = 0; i < models_paths.size();i++){	
	meshes[i]->texture_path = textures_paths[i];
	}
	
	pipeline_data data_static_mesh = {};
	#ifdef VULKAN
    data_static_mesh.draw_type = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    #endif
	data_static_mesh.mesh_type = MESH_TYPE_STATIC;
    data_static_mesh.fragment_shader_path = "Game/Assets/shaders/frag.spv";
    data_static_mesh.vertex_shader_path = "Game/Assets/shaders/vert.spv";


	pipeline_data data_skinned_mesh = {};
	#ifdef VULKAN
    data_skinned_mesh.draw_type = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	#endif
    data_skinned_mesh.mesh_type = MESH_TYPE_SKINNED;
    data_skinned_mesh.fragment_shader_path = "Game/Assets/shaders/red.spv";
    data_skinned_mesh.vertex_shader_path = "Game/Assets/shaders/skin.spv";

	for(EMesh* mesh : meshes){
		mesh->data = data_static_mesh;
	}
	for(int id : skeletal_id){//assing skinned shader
		meshes[id]->data = data_skinned_mesh;
	}
}


void Engine::update_input(){
		if(input.TAB.Released){
					if(!edit_mode)
						edit_mode = true;
					else
					{
						edit_mode = false;
					}
					input.TAB.Released = false;
#ifdef DEVELOPMENT
					this->print_debug("tab pressed or released",0,0);
#endif
		}

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

