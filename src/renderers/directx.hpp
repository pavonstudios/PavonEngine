#ifndef DIRECTX
#define DIRECTX
#include <d3d11.h>



#pragma comment (lib, "d3d11.lib")

class Engine;

class Renderer{
public:
	Engine* engine;
	// global declarations
	IDXGISwapChain* swapchain;             // the pointer to the swap chain interface
	ID3D11Device* dev;                     // the pointer to our Direct3D device interface
	ID3D11DeviceContext* devcon;           // the pointer to our Direct3D device context
	ID3D11RenderTargetView* backbuffer;    // global declaration
	void init();
	void draw_frame();
};
#endif