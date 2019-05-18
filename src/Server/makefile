CC = g++ -std=c++17 -g
main: server_data
	$(CC) main_server.cpp -o server server.o -lpthread

server_data:
	$(CC) -c server.cpp