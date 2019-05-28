#pragma once

#include <d3d12.h>
#include <DXGI1_5.h>

#include <wrl.h>

using namespace Microsoft::WRL;

class Engine;
class Renderer
{
public:
	Engine* engine;

	ID3D12Device1* device;
	ID3D12CommandQueue* command_queue;
	ComPtr<IDXGISwapChain3> swap_chain;
	void init();
	void get_hardware_adapter(IDXGIFactory4* pFactory, IDXGIAdapter1** ppAdapter);
};

