#include "connectivity.hpp"

#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <iostream>
#include <thread>

#include <cstring>
#include "../engine.h"
#include "../Game/ThirdPerson.hpp"
#include "../Server/client.hpp"
#include "../Game/game.hpp"

void ConnectionManager::connect_to_game_server(){
	init();	

	server_socket = socket(AF_INET, SOCK_STREAM,0);

	struct sockaddr_in ipOfServer;

	ipOfServer.sin_family = AF_INET;
    ipOfServer.sin_port = htons(6000);
    ipOfServer.sin_addr.s_addr = inet_addr("127.0.0.1");

	int connected = connect(server_socket, (struct sockaddr *)&ipOfServer, sizeof(ipOfServer));
	if(connected < 0){
		std::cout << "not connected\n";
		server_socket = -1;
	}

	
	std::string message = "hello server";
	char msg[1000];
	memset(&msg,0,sizeof(msg));
	strcpy(msg, message.c_str());
	//send(new_socket,(char*)&msg, strlen(msg),0);


	
	
}

ConnectionManager::ConnectionManager(){

}

ConnectionManager::~ConnectionManager(){
	ClientPacket packet {};
	packet.command = COMMAND_EXIT;
	send(server_socket,&packet,sizeof(ClientPacket),0);
}
/*
Accept connections from server
*/
void ConnectionManager::init(){
	std::thread t_wait_data(wait_data,this);
	t_wait_data.detach();
}

void ConnectionManager::wait_data(ConnectionManager* manager){
	int socket_server_file_descriptor = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in address;
	int addrlen = sizeof(address);

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(6001);

	bind(socket_server_file_descriptor,(struct sockaddr*)&address,sizeof(address));

	listen(socket_server_file_descriptor,3);

	
	std::cout << "waiting server response\n";
	manager->in_server_socket = accept(socket_server_file_descriptor,(struct sockaddr *)&address,(socklen_t*)&addrlen);
	std::cout << "connectd\n";
	std::thread recieve_d(ConnectionManager::recieve_data,manager);
	recieve_d.detach();
}
void ConnectionManager::recieve_data(ConnectionManager* manager){
	std::cout << "recieving data from server\n";
	while(true){
		manager->update();
	}
}
void ConnectionManager::send_player_data(ThirdPerson* player){
	ClientPacket packet {};

	packet.command = COMMAND_MOVE;
	packet.position = player->mesh->location_vector;

	if(server_socket > -1)
		send(server_socket,&packet,sizeof(ClientPacket),0);
}

void ConnectionManager::update(){

	SendPacket packet = {};
	recv(in_server_socket,&packet,sizeof(SendPacket),0);
	if(packet.players_count > 0){
		if(!player_spawned){
			game->spawn_new_player();
			player_spawned = true;
		}
		engine->print_vector(packet.other_player_position);
		game->player2->location = packet.other_player_position;
	}

}