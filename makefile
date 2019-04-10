CLANG=clang++ -g -std=c++17 -stdlib=libc++
CC=g++ -g -std=c++17
Library=-lglfw -lpthread -lm

MAIN_OBJS = camera.cpp engine.cpp gltf.cpp
DEFINES= -DGLTF -DTINYGLTF_NO_EXTERNAL_IMAGE

.ONESHELL:
main: main.o renderer.o engine.o camera.o
	mkdir -p bin && cd src
	$(CC) -o ../renderer main.o renderer.o engine.o camera.o $(Library) -I./ -lvulkan  -DVULKAN $(DEFINES)

.ONESHELL:
main.o:
	cd src
	$(CC) -c main.cpp -DGLTF -DVULKAN -DTINYGLTF_NO_EXTERNAL_IMAGE

.ONESHELL:
renderer.o:
	cd src
	$(CC) -c renderer.cpp -I./ -DGLTF -DVULKAN

.ONESHELL:
engine.o: 
	cd src
	$(CC) -c engine.cpp -I./ -DGLTF -DVULKAN
	
.ONESHELL:
camera.o:
	cd src
	$(CC) -c camera.cpp gltf.cpp -I./ -DGLTF -DVULKAN

.ONESHELL:
gl:
	mkdir -p bin && cd src
	$(CC) $(MAIN_OBJS) opengl_renderer.cpp -o ../renderer $(Library) -lGLEW -lGL -I./ -D_OpenGL_Renderer_

.ONESHELL:
clang:
	mkdir -p bin && cd src
	$(CLANG) $(MAIN_OBJS) renderer.cpp -o ../renderer $(Library) -I./ -lvulkan  -DVULKAN $(DEFINES)
