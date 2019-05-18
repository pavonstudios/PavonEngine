#ifndef CLIENT_H
#define CLIENT_H

#define COMMAND_EXIT 5
#define COMMAND_MOVE 1

#include <glm/glm.hpp>

struct Client{
	int id = -1;
	int client_socket = -1;
	bool connected = false;
};

struct ClientPacket{
	int command = 0;
	glm::vec3 position;
};
#endif