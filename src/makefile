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

COMPILE= $(CC) -c -DGLTF $(DEFINES) -DDEVELOPMENT

GAME := Game/*.o
GAME += Multiplayer/*.o

INCLUDE_OPENGL = -lGLESv2 -lEGL -lX11 

DEFINES :=
TYPE :=



vk: DEFINES := -DVULKAN
vk: Library += -lglfw
vk: TYPE := vk
vk: CC := $(CCLINUX)
vk: renderer.o  $(OBJs) game
	
	$(CC) -o ../renderer renderer.o audio.o $(OBJs) $(GAME) $(Library) -I./ -lvulkan $(DEFINES)



es2: DEFINES := -DES2
es2: DEFINES += -DDEVELOPMENT
es2: TYPE := es2
es2: CC := $(CCLINUX)
es2: $(OBJs) game connectivity opengl.o	
	$(CC) -o ../renderer $(OBJs) audio.o ./renderers/opengl.o $(GAME) $(Library) $(INCLUDE_OPENGL) -I./ $(DEFINES) -DGLTF 


win: CC := $(CCWIN)
win: DEFINES := -DWINDOWS
win: WindowManager.o
	
	$(CCWIN) main.cpp -o ../renderer.exe WindowManager.o -lglfw3 -lopengl32 -lgdi32 -static -static-libstdc++ -DWINDOWS



link:
	
	$(CC) -o ../renderer $(OBJs) audio.o model_loader.o $(GAME) $(Library) $(INCLUDE_OPENGL) -I./ $(DEFINES) -DGLTF -lX11 


.PHONY: game
game:
	$(MAKE) -C ./Game $(TYPE)


WindowManager.o: 
	
	$(CC) -c WindowManager.cpp -DGLTF $(DEFINES)

.ONESHELL:
opengl.o: 
	cd renderers
	$(CC) -c opengl.cpp -DGLTF $(DEFINES)

.ONESHELL:
connectivity: 
	cd Multiplayer
	$(CC) -c connectivity.cpp -DGLTF $(DEFINES)


animation_manager.o: 
	
	$(CC) -c animation_manager.cpp -DGLTF $(DEFINES)



audio_manager.o: audio.o
	
	$(CC) -c audio_manager.cpp -DGLTF $(DEFINES)


audio.o: 
	
	gcc -g -c audio.c -DGLTF $(DEFINES)


collision.o: 
	
	$(CC) -c collision.cpp -DGLTF $(DEFINES)


game_map.o: 
	
	$(CC) -c game_map.cpp -DGLTF $(DEFINES)


skeletal.o: 
	
	$(CC) -c skeletal.cpp -DGLTF $(DEFINES)


input.o: 
	
	$(CC) -c input.cpp -DGLTF $(DEFINES) -DDEVELOPMENT

model_loader.o: CC := $(CCLINUX)
model_loader.o: 
	
	$(CC) -c model_loader.cpp -DGLTF $(DEFINES)



objects.o:
	
	$(CC) -c objects.cpp -DGLTF $(DEFINES)



main.o:
	
	$(CC) -c main.cpp -DGLTF $(DEFINES)


renderer.o: ./src/renderer.cpp
	
	$(CC) -c renderer.cpp -I./ -DGLTF $(DEFINES)


asset_manager.o:
	
	$(CC) -c asset_manager.cpp -I./ -DGLTF $(DEFINES)


engine.o: 
	
	$(CC) -c engine.cpp -I./ -DGLTF $(DEFINES) -DDEVELOPMENT
	

camera.o:
	
	$(CC) -c camera.cpp -I./ -DGLTF $(DEFINES)
