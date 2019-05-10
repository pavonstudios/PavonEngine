CLANG=clang++ -g -std=c++17 -stdlib=libc++
CC=g++ -g -std=c++17 -Wall
Library=-lglfw -lpthread -lm -lSDL2

MAIN_OBJS = camera.cpp engine.cpp
DEFINES= -DGLTF -DDEVELOPMENT

OBJs= main.o engine.o camera.o asset_manager.o objects.o input.o skeletal.o WindowManager.o game_map.o collision.o audio_manager.o


COMPILE= $(CC) -c -DGLTF $(DEFINES) -DDEVELOPMENT

GAMEOBJs = $(wildcard /home/pavon/rt_renderer/src/Game/*.o)

GAME = Game/*.o

INCLUDE_OPENGL = -lGLEW -lGLESv2 -lEGL

DEFINES :=
TYPE :=


.ONESHELL:
vk: DEFINES := -DVULKAN
vk: TYPE := vk
vk: renderer.o  $(OBJs) game
	mkdir -p bin && cd src
	$(CC) -o ../renderer renderer.o audio.o $(OBJs) model_loader.o $(GAME) $(Library) -I./ -lvulkan $(DEFINES)


.ONESHELL:
es2: DEFINES := -DES2
es2: TYPE := es2
es2: $(OBJs) game
	mkdir -p bin && cd src
	$(CC) -o ../renderer $(OBJs) model_loader.o audio.o $(GAME) $(Library) $(INCLUDE_OPENGL) -I./ $(DEFINES) -DGLTF -lX11 

.ONESHELL:
link:
	cd ./src
	$(CC) -o ../renderer $(OBJs) audio.o model_loader.o $(GAME) $(Library) $(INCLUDE_OPENGL) -I./ $(DEFINES) -DGLTF -lX11 

.ONESHELL:
game:
	cd src/Game
	$(MAKE) $(TYPE) -j8

.ONESHELL:
WindowManager.o: 
	cd src
	$(CC) -c WindowManager.cpp -DGLTF $(DEFINES)


.ONESHELL:
audio_manager.o: audio.o
	cd src
	$(CC) -c audio_manager.cpp -DGLTF $(DEFINES)

.ONESHELL:
audio.o: 
	cd src
	gcc -g -c audio.c -DGLTF $(DEFINES)

.ONESHELL:
collision.o: 
	cd src
	$(CC) -c collision.cpp -DGLTF $(DEFINES)

.ONESHELL:
game_map.o: 
	cd src
	$(CC) -c game_map.cpp -DGLTF $(DEFINES)

.ONESHELL:
skeletal.o: 
	cd src
	$(CC) -c skeletal.cpp -DGLTF $(DEFINES)

.ONESHELL:
input.o: 
	cd src
	$(CC) -c input.cpp -DGLTF $(DEFINES) -DDEVELOPMENT

.ONESHELL:
model_loader.o: 
	cd src
	$(CC) -c model_loader.cpp -DGLTF $(DEFINES)


.ONESHELL:
objects.o:
	cd src
	$(CC) -c objects.cpp -DGLTF $(DEFINES)


.ONESHELL:
main.o:
	cd src
	$(CC) -c main.cpp -DGLTF $(DEFINES)

.ONESHELL:
renderer.o: ./src/renderer.cpp
	cd src
	$(CC) -c renderer.cpp -I./ -DGLTF $(DEFINES)

.ONESHELL:
asset_manager.o:
	cd src
	$(CC) -c asset_manager.cpp -I./ -DGLTF $(DEFINES)

.ONESHELL:
engine.o: 
	cd src
	$(CC) -c engine.cpp -I./ -DGLTF $(DEFINES) -DDEVELOPMENT
	
.ONESHELL:
camera.o:
	cd src
	$(CC) -c camera.cpp -I./ -DGLTF $(DEFINES)

