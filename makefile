CC=g++ -g
Library=-lvulkan -lglfw


main: 
	$(CC) main.cpp -o renderer $(Library)

