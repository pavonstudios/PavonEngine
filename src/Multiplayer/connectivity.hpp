#ifndef CONECTIVITY
#define CONECTIVITY
class ThirdPerson;
class Game;
class Engine;
class ConnectionManager{
	public:
		int server_socket = -1;
		int in_server_socket = -1;
		Engine* engine;
		Game* game;
		ConnectionManager();
		~ConnectionManager();
		void connect_to_game_server();
		void init();
		static void wait_data(ConnectionManager*);
		void send_player_data(ThirdPerson* player);
		void update();
		static void recieve_data(ConnectionManager*);
		bool player_spawned = false;
};
#endif