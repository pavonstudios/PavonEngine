#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include "client.hpp"
class Server{
public:
	std::vector<Client*> clients;
};
#endif