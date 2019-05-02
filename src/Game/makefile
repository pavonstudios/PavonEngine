CC=g++ -g -std=c++17

SRCS := $(wildcard *.cpp)
BINS := $(SRCS:%.cpp=%)

DEFINE := 

vk: DEFINE := -DVULKAN
vk: ${BINS}

es2: DEFINE := -DES2
es2: ${BINS}

%: %.cpp
	$(CC) -c $< -DGLTF $(DEFINE) -DDEVELOPMENT

	