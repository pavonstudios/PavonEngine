#include "connectivity.hpp"
void ConnectionManager::connect_to_game_server(){
	init();
	
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
void ConnectionManager::init(){
	std::thread t_wait_data(wait_data);
	t_wait_data.detach();
}

void ConnectionManager::wait_data(){
	int socket_server_file_descriptor = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in address;
	int addrlen = sizeof(address);

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(6001);

	bind(socket_server_file_descriptor,(struct sockaddr*)&address,sizeof(address));

	listen(socket_server_file_descriptor,3);

	int client_conection = 0;
	std::cout << "waiting server response\n";
	while(1){
		client_conection = accept(socket_server_file_descriptor,(struct sockaddr *)&address,(socklen_t*)&addrlen);
		std::cout << "server responce\n"; 
	}
}