#ifndef CONECTIVITY
#define CONECTIVITY

class ConnectionManager{
	public:
		ConnectionManager();
		void connect_to_game_server();
		void init();
		static void wait_data();
};
#endif