#include "../engine.h"
#include "dx12.hpp"


void Renderer::init() {
	ID3D12Debug* debug_controller;
	if ( SUCCEEDED( D3D12GetDebugInterface( IID_PPV_ARGS(&debug_controller) ) ) ) {
		debug_controller->EnableDebugLayer();
	}

	IDXGIFactory5* factory;
	HRESULT result;
	result = CreateDXGIFactory1(__uuidof(IDXGIFactory5), (void**)(&factory));

	IDXGIAdapter1* hardware_adapter;
	
	get_hardware_adapter(factory, &hardware_adapter);

	result = D3D12CreateDevice(hardware_adapter, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device1), (void**)(&device) );

	D3D12_COMMAND_QUEUE_DESC command_queue_description;
	command_queue_description.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	command_queue_description.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	command_queue_description.NodeMask = 0;
	command_queue_description.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;

	result = device->CreateCommandQueue(&command_queue_description, __uuidof(ID3D12CommandQueue), (void**)(&command_queue));

	DXGI_SWAP_CHAIN_DESC swap_chain_desc = {};
	swap_chain_desc.BufferCount = 2;
	swap_chain_desc.BufferDesc.Width = 800;
	swap_chain_desc.BufferDesc.Height = 600;
	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swap_chain_desc.OutputWindow = engine->window_manager.window_handler;
	swap_chain_desc.SampleDesc.Count = 1;
	swap_chain_desc.Windowed = TRUE;

	ComPtr<IDXGISwapChain> temp_swap_chain;

	factory->CreateSwapChain(command_queue, &swap_chain_desc, &temp_swap_chain);

	temp_swap_chain.As(&swap_chain);

	int frame_index = swap_chain->GetCurrentBackBufferIndex();



}

void Renderer::get_hardware_adapter(IDXGIFactory4* pFactory, IDXGIAdapter1** ppAdapter)
{
	*ppAdapter = nullptr;
	for (UINT adapterIndex = 0; ; ++adapterIndex)
	{
		IDXGIAdapter1* pAdapter = nullptr;
		if (DXGI_ERROR_NOT_FOUND == pFactory->EnumAdapters1(adapterIndex, &pAdapter))
		{
			// No more adapters to enumerate.
			break;
		}

		// Check to see if the adapter supports Direct3D 12, but don't create the
		// actual device yet.
		if (SUCCEEDED(D3D12CreateDevice(pAdapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
		{
			*ppAdapter = pAdapter;
			return;
		}
		pAdapter->Release();
	}
}