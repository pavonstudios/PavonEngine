#include <iostream>
#include <thread>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h> 

bool quit = false;

void input(){
	std::cout << "input thread created\n";
	std::string input;
	while(!quit){
		std::cin >> input;
		if(input == "q"){
			std::cout << "exist request\n";
			quit = true;
			
		}
	}	
	
}
void wait_conectiions(){
	int socket_server_file_descriptor = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in address;
	int addrlen = sizeof(address);

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(6000);

	bind(socket_server_file_descriptor,(struct sockaddr*)&address,sizeof(address));

	listen(socket_server_file_descriptor,3);

	int client_conection = 0;
	
	while(!quit){
		std::cout << "waiting connections\n";
		client_conection = accept(socket_server_file_descriptor,(struct sockaddr *)&address,(socklen_t*)&addrlen);
		
	}
}
int main(){
	
	std::thread t1(input);
	t1.detach();
	std::thread server(wait_conectiions);
	server.detach();
	while(!quit){

	}


}