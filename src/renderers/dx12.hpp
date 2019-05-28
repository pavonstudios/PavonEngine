#pragma once

#include <d3d12.h>
#include <DXGI1_5.h>


class Engine;
class Renderer
{
public:
	Engine* engine;

	ID3D12Device1* device;
	void init();
	void get_hardware_adapter(IDXGIFactory4* pFactory, IDXGIAdapter1** ppAdapter);
};

