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
		std::cout << "not connected\n";
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
		client_conection = accept(socket_server_file_descriptor,(struct sockaddr *)&address,(socklen_t*)&addrlen);
		Client* new_client = new Client;
		new_client->id = client_count;
		server->clients.push_back(new_client);
		std::cout << "someone connected\n";
		char msg[1000];
		//recv(client_conection,(char*)&msg,sizeof(msg),0);
		std::cout << msg << std::endl;
		glm::vec3 position;
		recv(client_conection,&position,sizeof(glm::vec3),0);
		std::cout << position.x << " " << position.y << " " << position.z << std::endl;

		server->send_data();
		client_count++;
	}

}