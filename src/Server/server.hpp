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

class Server{
public:
	std::vector<Client*> clients;
	void send_data();
	void get_ip_client();
};
#endif