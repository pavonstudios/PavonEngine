CC=g++ -g -std=c++17

SRCS := $(wildcard *.cpp)
BINS := $(SRCS:%.cpp=%)

OBJS := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

CPPS := $(patsubst %.o,%.cpp,$(wildcard *.o))

DEFINE := 

vk: DEFINE := -DVULKAN
vk: game

es2: DEFINE := -DES2
es2: game

.PHONY: game
game: ${OBJS}


%.o: %.cpp
	$(CC) -c $< -DGLTF $(DEFINE) -DDEVELOPMENT

.PHONY:clear
clean:
	rm -f *.o
	