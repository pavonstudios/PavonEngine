#ifndef COLLISION
#define COLLISION

#include <vector>
class Engine;

namespace engine{

struct EMesh;


}

class Collision{
public:
	static bool collision_of(EMesh* mesh2 , EMesh* mesh);

	static bool sphere_collision_of(EMesh* mesh2 , EMesh* mesh);

	static void update_collision(EMesh* mesh, EMesh* mesh2);
	static void update_collision(const std::vector<EMesh*>& meshes);
	static void update_collision_engine(const Engine* engine);

	static bool trigger_on(EMesh* mesh2 , EMesh* mesh);

	static void create_collision_helper_vertices(Engine*);
};
#endif