CC=g++ -g -std=c++17
Library=-lvulkan -lglfw -lpthread

all: main

main: 
	$(CC) renderer.cpp main.cpp -o renderer $(Library) -I./

renderer.o:
	$(CC) -c renderer.cpp -I./
clear:
	rm -rf *.o

