#ifndef COLLISION
#define COLLISION
#include "objects.h"

using namespace engine;
class Collision{
public:
	static bool collision_of(EMesh* mesh2 , EMesh* mesh);

	static void update_collision(EMesh* mesh, EMesh* mesh2);
};
#endif