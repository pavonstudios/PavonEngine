CCLINUX=g++ -g -std=c++17 -Wall
CC = $(CCLINUX)

vk: vulkan.o

vulkan.o: vulkan.cpp vulkan.hpp
	$(CC) -c vulkan.cpp -DX11 -DVK_USE_PLATFORM_XLIB_KHR -DVULKAN -DGLTF

gl: opengl.cpp opengl.hpp
	$(CC) -c opengl.cpp