#include "../engine.h"
#include "dx12.hpp"



void Renderer::init() {
	ID3D12Debug* debug_controller;
	if ( SUCCEEDED( D3D12GetDebugInterface( IID_PPV_ARGS(&debug_controller) ) ) ) {
		debug_controller->EnableDebugLayer();
	}

	IDXGIFactory5* factory;
	HRESULT hr = CreateDXGIFactory1(__uuidof(IDXGIFactory5), (void**)(&factory));

	IDXGIAdapter1* hardware_adapter;
	
	get_hardware_adapter(factory, &hardware_adapter);

	HRESULT r = D3D12CreateDevice(hardware_adapter, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device1), (void**)(&device) );





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