#include "connectivity.hpp"
void ConnectionManager::connect_to_game_server(){

	int new_socket;

	new_socket = socket(AF_INET, SOCK_STREAM,0);

	struct sockaddr_in ipOfServer;

	ipOfServer.sin_family = AF_INET;
    ipOfServer.sin_port = htons(6000);
    ipOfServer.sin_addr.s_addr = inet_addr("127.0.0.1");

	int connected = connect(new_socket, (struct sockaddr *)&ipOfServer, sizeof(ipOfServer));
	if(connected < 0){
		std::cout << "not connected\n";
	}

}

ConnectionManager::ConnectionManager(){

}