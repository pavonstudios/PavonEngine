#ifndef CONECTIVITY
#define CONECTIVITY
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <iostream>
#include <thread>
class ConnectionManager{
	public:
		ConnectionManager();
		void connect_to_game_server();
		void init();
		static void wait_data();
};
#endif