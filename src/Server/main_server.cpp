#include <iostream>
#include <thread>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h> 

#include "server.hpp"

bool quit = false;

void input(Server* server){
	std::cout << "input thread created\n";
	std::string input;
	while(!quit){
		std::cin >> input;
		if(input == "q"){
			std::cout << "exist request\n";
			quit = true;
			
		}
		if(input == "clients"){
			std::cout << "Client count: " << server->clients.size() << std::endl;
			for(Client* client : server->clients){

			}
		}
		if(input == "send"){
			server->send_data();			
		}
	}	
	
}
void wait_conectiions(Server* server){
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
	while(!quit){
		std::cout << "waiting connections\n";
		client_conection = accept(socket_server_file_descriptor,(struct sockaddr *)&address,(socklen_t*)&addrlen);
		Client* new_client = new Client;
		new_client->id = client_count;
		server->clients.push_back(new_client);
		std::cout << "someone connected\n";
		char msg[1000];
		recv(client_conection,(char*)&msg,sizeof(msg),0);
		std::cout << msg << std::endl;
		server->send_data();
		client_count++;
	}
}
int main(){
	Server server;

	std::thread t1(input,&server);
	t1.detach();
	std::thread server_thread(wait_conectiions,&server);
	server_thread.detach();
	server.get_ip_client();
	while(!quit){

	}


}