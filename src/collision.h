#ifndef COLLISION
#define COLLISION
#include "engine.h"

using namespace engine;
class Collision{
public:
	static bool collision_of(EMesh* mesh2 , EMesh* mesh);

	static void update_collision(EMesh* mesh, EMesh* mesh2);
	static void update_collision(const std::vector<EMesh*>& meshes);
	static void update_collision_engine(const Engine* engine);

	static bool trigger_on(EMesh* mesh2 , EMesh* mesh);
};
#endif