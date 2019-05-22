CC=g++ -g -std=c++17 $(CFLAGS)

SRCS := $(wildcard *.cpp)
BINS := $(SRCS:%.cpp=%)

OBJS := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

CPPS := $(patsubst %.o,%.cpp,$(wildcard *.o))

DEFINES := 

vk: DEFINES := -DVULKAN
vk: game

es2: DEFINES := -DES2
es2: DEFINES += -DLINUX
es2: DEFINES += -DX11
es2: game

.PHONY: game
game: ${OBJS}


%.o: %.cpp %.hpp
	$(CC) -c $< -DGLTF $(DEFINES) -DDEVELOPMENT

.PHONY:clear
clean:
	rm -f *.o
	