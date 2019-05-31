SRCS := $(wildcard *.cpp)
BINS := $(SRCS:%.cpp=%)

OBJS := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

CPPS := $(patsubst %.o,%.cpp,$(wildcard *.o))



vk: game

es2: game

.PHONY: game
game: ${OBJS}


%.o: %.cpp %.hpp
	$(CC) -c $< -DGLTF $(DEFINES) -DDEVELOPMENT

.PHONY:clear
clean:
	rm -f *.o
	