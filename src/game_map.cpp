#include "engine.h"
#include "game_map.hpp"
#include "Game/game.hpp"

#include <thread>
void MapManager::parse_map_file(std::stringstream &file){
		std::string line;
		int counter = 0;		
		//int type = 0;		
	

		while( std::getline(file,line) ) {		

			if(line != ""){
				char first_char;
				std::stringstream line_stream (line);
			
				std::string model_path;
				glm::vec3 location;
				std::string texture_path;
				std::string type;
				int mesh_type = 0;
				MapDataToLoad data = {};

				line_stream >> first_char;			
				if(first_char == '/'){
					break;
				}
				if(first_char == 'v'){
					line_stream >> texture_path >> model_path;					
					same_textures.push_back(texture_path);
					meshes_paths.push_back(model_path);
					data.model_path = model_path;
					data.texture_path = texture_path;
					unique_model_data.push_back(data);
				}
				if(first_char != '#' && first_char != 'v'){
					

					line_stream >> model_path >> location.x >> location.y >> location.z >> texture_path >> type;
					if(type == "LOD"){
						mesh_type = MESH_LOD;
					}
					if(first_char == 'c'){
						texture_path = "textures/car01.jpg";
					}
					if(first_char == 's'){
						skeletal_id.push_back(counter);
					}
					if(first_char == 'a'){
						//with collider
						mesh_type = MESH_WITH_COLLIDER;
					}
					
					if (player_id == -1) {
						if (type == "player") {
							player_id = counter;
						}
					}

					if (engine->game->player_id == -1) {
						if (type == "player") {
							engine->game->player_id = counter;

						}
					}

					counter++;								
					
					data.location = location;
					data.type = mesh_type;

					if(first_char == 'b'){
						texture_path = same_textures[0];
						data.texture_id = 0;
						data.model_id = 0;
						
					}else{
						data.texture_id = -1;
						data.model_path = model_path;
						data.texture_path = texture_path;						
					}		

					meshes_load_data.push_back(data);
				}			
			
			}				
		
		}

		
}
void MapManager::load_primitives()
{
	for (EMesh* mesh : engine->meshes) {
		engine->mesh_manager.load_primitives_data(mesh, mesh->gltf_model);
	}
}
void MapManager::load_model_to_cpu_memory()
{
	int core_count = std::thread::hardware_concurrency();
	int meshes_count = model_load_data.size();

	div_t division = std::div(meshes_count, core_count);

	std::vector< std::vector<ModelLoadData*> > parts;

	int cout_to_div = division.quot;
	int counter = 0;
	std::vector<ModelLoadData*> part;
	for (int i = 0; i < model_load_data.size() - 1; i++) {

		part.push_back(&model_load_data[i]);
		counter++;
		if (counter == cout_to_div) {

			parts.push_back(part);
			counter = 0;
			part.clear();
		}
	}
	if (division.rem >= 1) {
		std::vector<ModelLoadData*> part;
		part.push_back(&model_load_data.back());
		parts.push_back(part);
	}

	std::vector<std::thread> threads;
	for (int t = 0; t < parts.size(); t++) {
		std::thread load(MeshManager::load_model, std::ref(parts[t]) ,  this->engine);
		threads.push_back(std::move(load));
	}
	for (int v = 0; v < threads.size(); v++) {
		threads[v].join();
	}
}
void MapManager::load_meshes_for_instance(struct MapDataToLoad &data){
	std::string path = engine->assets.path(data.model_path);

	#ifdef VULKAN
		EMesh *model = new EMesh(engine->vulkan_device);//vulkan device for create vertex buffers
		model->texture.format = VK_FORMAT_R8G8B8A8_UNORM;			
	#else
		EMesh *model = new EMesh();
    #endif
	#ifdef ANDROID
		engine->mesh_manager.load_mode_gltf_android(model,path.c_str(),engine->pAndroid_app->activity->assetManager);
	#else
		engine->mesh_manager.load_model_gltf(model, path.c_str());
	#endif

	//model->name = data.model_path;

	engine->unique_meshes.push_back(model);
}

void MapManager::create_mesh_with_data(struct MapDataToLoad &data){
	std::string path;
	bool load_vertices = true;
	if(data.model_path != "")
		path = engine->assets.path(data.model_path);
	else
	{
		load_vertices = false;
		data.model_path  = unique_model_data[0].model_path;
	}
	
	vec3 location = data.location;

	#ifdef VULKAN
		EMesh *model = new EMesh(engine->vulkan_device);//vulkan device for create vertex buffers
		model->texture.format = VK_FORMAT_R8G8B8A8_UNORM;			
	#else
		EMesh *model = new EMesh();
    #endif

	if(load_vertices){
			#ifdef ANDROID
				engine->mesh_manager.load_mode_gltf_android(model,path.c_str(),engine->pAndroid_app->activity->assetManager);
			#else
				engine->mesh_manager.load_model_gltf(model, path.c_str());
			#endif	
	}

	//model->name = data.model_path;

	glm::mat4 model_matrix = glm::mat4(1.0f);
	model_matrix = glm::translate(model_matrix, location);
	model->location_vector = location;
	model->model_matrix = model_matrix;

	model->type = data.type;
	if(data.type != MESH_LOD){
		engine->meshes.push_back(model);
		
	}
	model->texture_path = engine->assets.path(data.texture_path);
	model->texture.texture_id = data.texture_id;
	model->model_id = data.model_id;
	engine->linear_meshes.push_back(model);


}

void MapManager::create_meshes_with_data(std::vector<MapDataToLoad>& data_pack)
{
	for (MapDataToLoad& data : data_pack) {
		std::string path;
		
		
		path = engine->assets.path(data.model_path);
		

		vec3 location = data.location;

#ifdef VULKAN
		EMesh* model = new EMesh(engine->vulkan_device);//vulkan device for create vertex buffers
		model->texture.format = VK_FORMAT_R8G8B8A8_UNORM;
#else
		EMesh* model = new EMesh();
#endif

#ifdef ANDROID
			engine->mesh_manager.load_mode_gltf_android(model, path.c_str(), engine->pAndroid_app->activity->assetManager);
#else
		ModelLoadData model_load;
		model_load.mesh_to_load = model;
		model_load.path = path;
		model_load_data.push_back(model_load);
		//engine->mesh_manager.load_model_gltf(model, path.c_str());
#endif	
		
		model->location_vector = location;
		model->model_matrix = glm::translate(glm::mat4(1.0f), location);

		model->type = data.type;

		engine->meshes.push_back(model);
	
		model->texture_path = engine->assets.path(data.texture_path);
		model->texture.texture_id = data.texture_id;
		model->model_id = data.model_id;
		engine->linear_meshes.push_back(model);


	}
}

void MapManager::assign_shader_path(){
	//assign shaders paths

		#ifdef VULKAN
			PipelineData data_static_mesh = {};
			
			data_static_mesh.draw_type = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			
			data_static_mesh.mesh_type = MESH_TYPE_STATIC;
			data_static_mesh.fragment_shader_path = "Game/Assets/shaders/vk/frag.spv";
			data_static_mesh.vertex_shader_path = "Game/Assets/shaders/vk/vert.spv";		

			for(EMesh* mesh : engine->linear_meshes){
				mesh->data_shader = data_static_mesh;
			}		
	

			
			
		#endif

		PipelineData data = {};

			#if defined (ES2) || defined (OPENGL)
				data.fragment_shader_path = engine->assets.path("shaders/gles/frag.glsl");
				data.vertex_shader_path = engine->assets.path("shaders/gles/vert_mvp.glsl");			
			#endif

			#ifdef ANDROID
				data.fragment_shader_path = engine->assets.path("shaders/gles/uv.glsl");
				data.vertex_shader_path = engine->assets.path("shaders/gles/triangle_vert_shader.glsl");
			#endif		

			

			for(EMesh* mesh : engine->linear_meshes){
				if(mesh->data_shader.vertex_shader_path == ""){
					if(mesh->model_id == -1)
						mesh->data_shader = data;
					else
					{
						mesh->data_shader = data;
						mesh->data_shader.fragment_shader_path = engine->assets.path("shaders/gles/red.glsl");
					}
						
				}
			}

			PipelineData data_skinned_mesh = {};
			#ifdef VULKAN		
			data_skinned_mesh.draw_type = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			data_skinned_mesh.fragment_shader_path = "Game/Assets/shaders/vk/frag.spv";
			data_skinned_mesh.vertex_shader_path = "Game/Assets/shaders/vk/skin_shader.spv";
			#endif
			#if defined(ES2) || defined(ANDROID) || defined (OPENGL)
			data_skinned_mesh.fragment_shader_path = engine->assets.path("shaders/gles/frag.glsl");
			data_skinned_mesh.vertex_shader_path = engine->assets.path("shaders/gles/skin_vert.glsl");
			#endif

			data_skinned_mesh.mesh_type = MESH_TYPE_SKINNED;	

			for(int id : skeletal_id){//assing skinned shader
				engine->linear_meshes[id]->data_shader = data_skinned_mesh;
				engine->linear_meshes[id]->type = MESH_TYPE_SKINNED;
			}
			
}

void MapManager::load_skeletal_meshes(){
	#ifndef WINDOWS
		for (int id : skeletal_id) {
			engine->skeletal_meshes.push_back(engine->linear_meshes[id]);
			SkeletalManager::load_data(&engine->animation_manager, engine->linear_meshes[id]);
		}
	#endif // 
}

void MapManager::create_meshes_with_map_loaded_data(){
		
		/*for(auto &data : unique_model_data){
			load_meshes_for_instance(data);
		}*/
		/*for(auto &data : meshes_load_data){
			create_mesh_with_data(data);
		}*/

		create_meshes_with_data(meshes_load_data);

		
}

void MapManager::load_file_map(std::string path) {
#ifndef ANDROID

	std::stringstream file;
	std::ifstream text_file(path);
	if (text_file)
	{
		file << text_file.rdbuf();
		text_file.close();
	}

#endif

#ifdef ANDROID
	AAsset* android_file = AAssetManager_open(pAndroid_app->activity->assetManager, path.c_str(), AASSET_MODE_BUFFER);

	size_t file_length = AAsset_getLength(android_file);
	char* fileContent = new char[file_length + 1];

	AAsset_read(android_file, fileContent, file_length);
	AAsset_close(android_file);

	std::stringstream file((std::string(fileContent)));

#endif

	if (!file)
	{
	#ifndef ANDROID
			throw std::runtime_error("failed to load map file");
	#endif
	#ifdef ANDROID
			LOGW("No file map");
	#endif
	}
	auto time_parse = std::chrono::high_resolution_clock::now();

	ENGINE_TIME(engine, parse_map_file(file), "parse map")


	ENGINE_TIME(engine, create_meshes_with_map_loaded_data(), "create meshes")

	ENGINE_TIME(engine, load_model_to_cpu_memory() , "model to cpu memory")

	ENGINE_TIME(engine , load_primitives() , "load primitives")

	assign_shader_path();
	load_skeletal_meshes();

	for (auto mesh : engine->meshes)
	{
		if (mesh->type == -1 || mesh->type == MESH_WITH_COLLIDER)
		{
			if (mesh->gltf_model.accessors.size() > 0)
			{
				if (mesh->gltf_model.accessors[0].minValues.size() > 0)
				{
					mesh->box.m_vecMax.x = mesh->gltf_model.accessors[0].maxValues[0];
					mesh->box.m_vecMax.y = mesh->gltf_model.accessors[0].maxValues[1];
					mesh->box.m_vecMax.z = mesh->gltf_model.accessors[0].maxValues[2];

					mesh->box.m_vecMin.x = mesh->gltf_model.accessors[0].minValues[0];
					mesh->box.m_vecMin.y = mesh->gltf_model.accessors[0].minValues[1];
					mesh->box.m_vecMin.z = mesh->gltf_model.accessors[0].minValues[2];
				}
			}
		}
	}
}

