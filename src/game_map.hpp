#ifndef GAMEMAPS
#define GAMEMAPS
class Engine;

#include <sstream>
#include <iostream>
#include <fstream>

#include "objects.h"
using namespace engine;

struct Map{

};

class MapManager{
public:
	Engine * engine;
	void load(std::string path);
	void load_data_from_file(std::stringstream & file);
	void create_mesh_with_data(load_data data);
	std::vector<std::string> same_textures;
};
#endif