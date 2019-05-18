#include "server.hpp"

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