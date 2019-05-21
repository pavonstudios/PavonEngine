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
		void update();
		void init_player();
		GUI* gui = nullptr;
		void init();
		ThirdPerson * player;
		int player_id = -1;
};
#endif