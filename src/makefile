CLANG=clang++ -g -std=c++17 -stdlib=libc++
CC:=
CCLINUX=g++ -g -std=c++17 -Wall
CCWIN=x86_64-w64-mingw32-g++ -g -std=c++17 -Wall
Library := -lpthread -lm -lSDL2

MAIN_OBJS = camera.cpp engine.cpp
DEFINES= -DGLTF -DDEVELOPMENT

OBJs := main.o engine.o camera.o asset_manager.o objects.o input.o skeletal.o WindowManager.o game_map.o collision.o audio_manager.o
OBJs += animation_manager.o
OBJs += model_loader.o
OBJs += gui.o


COMPILE= $(CC) -c -DGLTF $(DEFINES) -DDEVELOPMENT

GAME := Game/*.o


INCLUDE_OPENGL = -lGLESv2 -lEGL -lX11 

DEFINES :=
TYPE :=



vk: DEFINES := -DVULKAN
vk: Library += -lglfw
vk: TYPE := vk
vk: CC := $(CCLINUX)
vk: renderer.o  $(OBJs) game
	
	$(CC) -o ../renderer renderer.o audio.o $(OBJs) $(GAME) $(Library) -I./ -lvulkan $(DEFINES)


es2: DEFINES := -DLINUX
es2: DEFINES += -DES2
es2: DEFINES += -DX11
es2: DEFINES += -DDEVELOPMENT
es2: GAME += Multiplayer/*.o
es2: TYPE := es2
es2: CC := $(CCLINUX)
es2: $(OBJs) game connectivity.o opengl.o	
	$(CC) -o ../renderer audio.o $(OBJs) ./renderers/opengl.o $(GAME) $(Library) $(INCLUDE_OPENGL) -I./ $(DEFINES) -DGLTF 


#win: CC := $(CCWIN)
win: CC := $(CCLINUX)
win: DEFINES := -DWINDOWS
win: DEFINES += -DDEVELOPMENT
win: main.o engine.o WindowManager.o directx.o
	
	$(CC) -o ../renderer.exe main.o engine.o WindowManager.o directx.o -lglfw3 -lgdi32 -static -static-libstdc++ -DWINDOWS



link:
	
	$(CC) -o ../renderer $(OBJs) audio.o model_loader.o $(GAME) $(Library) $(INCLUDE_OPENGL) -I./ $(DEFINES) -DGLTF -lX11 


.PHONY: game
game:
	$(MAKE) -C ./Game $(TYPE)

gui.o: gui.hpp gui.cpp
	$(CC) -c gui.cpp -DGLTF $(DEFINES)


WindowManager.o: WindowManager.cpp WindowManager.hpp
	
	$(CC) -c WindowManager.cpp -DGLTF $(DEFINES)


opengl.o: ./renderers/opengl.cpp ./renderers/opengl.hpp
	$(CC) -c ./renderers/opengl.cpp -o ./renderers/opengl.o -DGLTF $(DEFINES)

directx.o: ./renderers/directx.cpp ./renderers/directx.hpp
	$(CC) -c ./renderers/directx.cpp -DGLTF $(DEFINES)

connectivity.o: ./Multiplayer/connectivity.cpp ./Multiplayer/connectivity.hpp
	$(CC) -c ./Multiplayer/connectivity.cpp -o ./Multiplayer/connectivity.o -DGLTF $(DEFINES)


animation_manager.o: animation_manager.cpp animation_manager.hpp skeletal.h
	
	$(CC) -c animation_manager.cpp -DGLTF $(DEFINES)



audio_manager.o: audio.o audio_manager.cpp audio_manager.hpp
	
	$(CC) -c audio_manager.cpp -DGLTF $(DEFINES)


audio.o: audio.c audio.h
	
	gcc -g -c audio.c -DGLTF $(DEFINES)


collision.o: 
	
	$(CC) -c collision.cpp -DGLTF $(DEFINES)


game_map.o: 
	
	$(CC) -c game_map.cpp -DGLTF $(DEFINES)


skeletal.o: 
	
	$(CC) -c skeletal.cpp -DGLTF $(DEFINES)


input.o: input.cpp input.h
	
	$(CC) -c input.cpp -DGLTF $(DEFINES) -DDEVELOPMENT

model_loader.o: 
	
	$(CC) -c model_loader.cpp -DGLTF $(DEFINES)



objects.o: objects.cpp objects.h
	
	$(CC) -c objects.cpp -DGLTF $(DEFINES)



main.o: main.cpp
	
	$(CC) -c main.cpp -DGLTF $(DEFINES)


renderer.o: ./src/renderer.cpp
	
	$(CC) -c renderer.cpp -I./ -DGLTF $(DEFINES)


asset_manager.o: asset_manager.h asset_manager.cpp
	
	$(CC) -c asset_manager.cpp -I./ -DGLTF $(DEFINES)


engine.o: engine.cpp engine.h
	
	$(CC) -c engine.cpp -I./ -DGLTF $(DEFINES) -DDEVELOPMENT
	

camera.o:
	
	$(CC) -c camera.cpp -I./ -DGLTF $(DEFINES)

