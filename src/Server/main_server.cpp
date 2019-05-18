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

int main(){
	Server server;

	std::thread t1(input,&server);
	t1.detach();
	std::thread server_thread(Server::wait_connections,&server);
	server_thread.detach();

	server.get_ip_client();
	while(!quit){
		
		if(server.can_replicate){
			//server.connect_to_clients();
			//server.replicate_clients_data();

		}
	}


}