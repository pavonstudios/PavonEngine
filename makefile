CC=g++ -g -std=c++17
Library=-lvulkan -lglfw -lpthread 


main:
	$(CC) engine.cpp 3D_objects.cpp renderer.cpp main.cpp -o renderer $(Library) -I./

renderer.o:
	$(CC) -c renderer.cpp -I./

gl:
	$(CC) engine.cpp main.cpp 3D_objects.cpp opengl_renderer.cpp -o renderer $(Library) -lGLEW -lGL -I./ -D_OpenGL_Renderer_
