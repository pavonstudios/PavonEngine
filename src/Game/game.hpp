#ifndef GAME
#define GAME


#include "ThirdPerson.hpp"	
#include "vehicle.hpp"
#include "../gui.hpp"


class Game{
	public:
		Game(Engine* engine);
		~Game();
		Engine* engine;
		EMesh* player_mesh;
		#ifdef ES2
		GUI* gui = nullptr;
		#endif // DEBUG	

		void update();
		void init_player();
		void init();
		void spawn_new_player();
		ThirdPerson * player = nullptr;
		ThirdPerson* player2 = nullptr;
		int player_id = -1;
};
#endif