CC = g++ -std=c++17 -g
main: server_data
	$(CC) main_server.cpp -o server server.o -lpthread

.PHONY: server_data
server_data: server.cpp client.hpp server.hpp
	$(CC) -c server.cpp