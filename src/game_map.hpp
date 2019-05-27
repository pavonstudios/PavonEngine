#ifndef GAMEMAPS
#define GAMEMAPS
class Engine;

#include <sstream>
#include <iostream>
#include <fstream>

#include "objects.h"
using namespace engine;
#include "engine_component.hpp"

struct Map{

};

class MapManager : public EngineComponent{
public:
	
	void create_meshes_with_map_loaded_data();
	void create_mesh_with_data(MapDataToLoad &data);
	void create_meshes_with_data(std::vector<MapDataToLoad> &data);
	void parse_map_file(std::stringstream &file);
	void load_primitives();

	void assign_shader_path();
	void load_meshes_for_instance(struct MapDataToLoad &data);
	void load_skeletal_meshes();
	void load_file_map(std::string path);

	std::vector<std::string> same_textures;
	std::vector<std::string> meshes_paths;
	std::vector<MapDataToLoad> meshes_load_data;
	std::vector<MapDataToLoad> unique_model_data;

	std::vector<int> skeletal_id;	

	int player_id = -1;
};
#endif