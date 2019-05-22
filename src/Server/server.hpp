#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include "client.hpp"
#include <iostream>
#include <thread>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <list>

class Server{
public:
	bool quit = false;
	bool can_replicate = true;
	bool can_delete = false;
	std::list<Client*> clients;
	void send_data();
	void get_ip_client();
	static void wait_connections(Server* server);
	static void recive_data(Server* , Client*);
	void replicate_clients_data();
	void connect_for_send_packets_to_clients();
	void send_to_client();
	int send_socket;
	void create_client_thread(Server* , Client* );
	int client_count = 0;
};
#endif