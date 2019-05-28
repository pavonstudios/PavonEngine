#ifndef DIRECTX
#define DIRECTX

#include <d3d11.h>
#include <d3dcompiler.h>


#pragma comment (lib, "d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

class Engine;

#include "..\objects.h"
using namespace engine;
class Renderer{
public:
	Engine* engine;

	IDXGISwapChain* swapchain;            
	ID3D11Device* dev;                    
	ID3D11DeviceContext* devcon;          
	ID3D11RenderTargetView* backbuffer;

	ID3D11Texture2D* texture_render_target = NULL;

	ID3D11Buffer* pVBuffer;

	ID3D11InputLayout* pLayout;
	
	ID3D11Buffer* uniform_buffer;

	ID3D11DepthStencilView* depthStencilView;
	ID3D11Texture2D* depthStencilBuffer;
	ID3D11DepthStencilState* pDepthStencilState;

	//textures
	ID3D11ShaderResourceView* CubesTexture;
	ID3D11SamplerState* CubesTexSamplerState;
	ID3D11Texture2D* texture = NULL;

	
	void init();
	void draw_frame();
	void update_constant_buffer(EMesh* mesh);
	void init_pipeline();
	void create_mesh_buffers(EMesh* mesh);
	void create_buffer(ID3D11Buffer **buffer);
	void load_texture(EMesh* mesh);
	HRESULT create_shader(LPCWSTR path, ID3DBlob** shader_blob, LPCSTR type, LPCSTR profile);
};
#endif