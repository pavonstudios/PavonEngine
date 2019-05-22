#include "server.hpp"
#include <glm/glm.hpp>

void Server::send_data(){
	int new_socket;

	new_socket = socket(AF_INET, SOCK_STREAM,0);

	struct sockaddr_in ipOfServer;

	ipOfServer.sin_family = AF_INET;
    ipOfServer.sin_port = htons(6001);
    ipOfServer.sin_addr.s_addr = inet_addr("127.0.0.1");//need to be cliend ip

	int connected = connect(new_socket, (struct sockaddr *)&ipOfServer, sizeof(ipOfServer));
	if(connected < 0){
		std::cout << "not connected to client\n";
	}
	send_socket = new_socket;
}

void Server::send_to_client(){
	SendPacket packet = {};
	packet.players_count = 1;
	send(send_socket,&packet,sizeof(SendPacket),0);
}
void Server::connect_to_clients(){

	for(Client* client : clients){
		
		if(client->send_connected){
			continue;
		}
		client->send_socket = socket(AF_INET, SOCK_STREAM,0);

		struct sockaddr_in ipOfServer;

		ipOfServer.sin_family = AF_INET;
		ipOfServer.sin_port = htons(6001);
		ipOfServer.sin_addr.s_addr = inet_addr("127.0.0.1");//need to be cliend ip

		std::cout << "connecting to send socket\n";
		int connected = connect(client->send_socket, (struct sockaddr *)&ipOfServer, sizeof(ipOfServer));
		if(connected < 0){
			std::cout << "not connected\n";
		}
		if(connected >= 0)
			client->send_connected = true;
	}

}

void Server::get_ip_client(){
	std::cout << "get ip\n";
}

void Server::wait_connections(Server* server){
	int socket_server_file_descriptor = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in address;
	int addrlen = sizeof(address);

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(6000);

	bind(socket_server_file_descriptor,(struct sockaddr*)&address,sizeof(address));

	listen(socket_server_file_descriptor,3);

	int client_conection = 0;
	int client_count = 0;
	while(!server->quit){
		std::cout << "waiting connections\n";
		Client* new_client = new Client;
		new_client->client_socket = accept(socket_server_file_descriptor,(struct sockaddr *)&address,(socklen_t*)&addrlen);
		server->can_replicate = false;

		new_client->id = client_count;
		new_client->connected = true;
		server->clients.push_back(new_client);

		std::thread new_client_thread (Server::recive_data,server,new_client);
		new_client_thread.detach();

		std::cout << "someone connected\n";

		char msg[1000];
		//recv(client_conection,(char*)&msg,sizeof(msg),0);
		std::cout << msg << std::endl;
		
		//server->send_data();
		client_count++;
		server->can_replicate = true;
	}

}

void Server::recive_data(Server* server, Client* client){
	while(client->connected){
		ClientPacket packet = {};
		recv(client->client_socket,&packet,sizeof(ClientPacket),0);
		
		client->position_recieved = packet.position;
		

		if( packet.command == COMMAND_EXIT ){
			client->connected = false;
			break;
		}
	}
	server->can_replicate = false;
	while(!server->can_delete){
		
	}
	server->clients.remove(client);
	server->can_replicate = true;
	std::cout << "client disconneted\n";

}

void Server::replicate_clients_data(){
	this->can_delete = false;
	for(Client* actual_client : clients){
		
		for(Client* send_client : clients){
			if(actual_client->id == send_client->id){
				continue;
			}
			if(actual_client->id >= 1){
			glm::vec3 position = actual_client->position_recieved;
			std::cout << position.x << " " << position.y << " " << position.z << std::endl;

			}
			SendPacket packet = {};
			packet.players_count = clients.size() - 1;
			packet.other_player_position = actual_client->position_recieved;
			send(actual_client->send_socket,&packet,sizeof(SendPacket),0);

		}

	}
	this->can_delete = true;
}