#ifndef GAME
#define GAME


#include "ThirdPerson.hpp"	
#include "vehicle.hpp"
#include "../gui.hpp"

class ObjectSelect;

class Game{
	public:
		Game();
		~Game();
		Engine* engine;
		EMesh* player_mesh;
		ObjectSelect* player_mesh_select;
		#ifdef ES2
		GUI* gui = nullptr;
		#endif // DEBUG	

		void update();
		void init_player();
		void init();
		void spawn_new_player();
		void spawn_new_mesh();
		bool mesh_spawned = false;
		ThirdPerson * player = nullptr;
		ThirdPerson* player2 = nullptr;
		int player_id = -1;

};
#endif