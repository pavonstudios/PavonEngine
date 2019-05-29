#ifndef DX9_HPP
#define DX9_HPP

#include <Windows.h>
#include <d3d9.h>

#pragma comment (lib, "d3d9.lib")
class Engine;

class Renderer{

public:
	Engine* engine;
	LPDIRECT3D9 d3d;    // the pointer to our Direct3D interface
	LPDIRECT3DDEVICE9 d3ddev;    // the pointer to the device class

	LPDIRECT3DVERTEXBUFFER9 vertex_buffer;

    void init();
	void draw_frame();
	
};

#endif