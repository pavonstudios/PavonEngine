CC=g++ -g -std=c++17
Library=-lvulkan -lglfw -lpthread -lGLEW -lGL

all: main

main: renderer.o
	$(CC) main.cpp -o renderer renderer.o $(Library) -I./

renderer.o:
	$(CC) -c renderer.cpp $(Library) -I./
clear:
	rm -rf *.o

