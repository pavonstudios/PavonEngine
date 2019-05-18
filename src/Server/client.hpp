#ifndef CLIENT_H
#define CLIENT_H
struct Client{
	int id = -1;
	int client_socket = -1;
	bool connected = false;
};
#endif