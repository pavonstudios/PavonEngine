CLANG=clang++ -g -std=c++17 -stdlib=libc++
CC=g++ -g -std=c++17
Library=-lglfw -lpthread -lm

MAIN_OBJS = camera.cpp engine.cpp
DEFINES= -DGLTF -DDEVELOPMENT

OBJs= main.o engine.o camera.o asset_manager.o 3D_objects.o input.o


COMPILE= $(CC) -c -DGLTF -DVULKAN -DDEVELOPMENT

GAMEOBJs = $(wildcard /home/pavon/rt_renderer/src/Game/*.o)

GAME = Game/*.o

INCLUDE_OPENGL = -lGLEW -lGL

DEFINES_OPENGL = D_OpenGL_Renderer_

DEFINES :=

.ONESHELL:
full: DEFINES := -DVULKAN
full: $(OBJs) game $(game) renderer.o WindowManager.o
	mkdir -p bin && cd src
	$(CC) -o ../renderer renderer.o $(OBJs) model_loader.o WindowManager.o $(GAME) $(Library) -I./ -lvulkan  -DVULKAN $(DEFINES)

.ONESHELL:
es2: DEFINES := -DES2
es2: engine.o WindowManager.o 3D_objects.o asset_manager.o camera.o input.o game $(game)
	mkdir -p bin && cd src
	$(CC) main.cpp -o ../renderer model_loader.o engine.o WindowManager.o 3D_objects.o asset_manager.o camera.o input.o $(GAME) $(Library) $(INCLUDE_OPENGL) -I./ $(DEFINES) -DGLTF -lEGL -lX11 -lGLESv2

.ONESHELL:
gl: DEFINES=-Dch 
gl: main.o 
	mkdir -p bin && cd src
	$(CC) -o ../renderer main.o


.ONESHELL:
WindowManager.o: 
	cd src
	$(CC) -c WindowManager.cpp -DGLTF $(DEFINES)

.ONESHELL:
input.o: 
	cd src
	$(CC) -c input.cpp -DGLTF $(DEFINES)

.ONESHELL:
model_loader.o: 
	cd src
	$(CC) -c model_loader.cpp -DGLTF $(DEFINES)

.ONESHELL:
3D_objects.o:
	cd src
	$(CC) -c 3D_objects.cpp -DGLTF $(DEFINES)

.ONESHELL:
game:
	cd src/Game
	$(foreach chori,*.cpp,$(COMPILE) $(chori))

.ONESHELL:
main.o:
	cd src
	$(CC) -c main.cpp -DGLTF $(DEFINES)

.ONESHELL:
renderer.o:
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


.ONESHELL:
clang:
	mkdir -p bin && cd src
	$(CLANG) $(MAIN_OBJS) renderer.cpp -o ../renderer $(Library) -I./ -lvulkan $(DEFINES)
