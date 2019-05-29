#include "../engine.h"
#include "dx12.hpp"

#include <d3dx12.h>

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

	D3D12_DESCRIPTOR_HEAP_DESC render_target_view_desc;
	render_target_view_desc.NumDescriptors = 2;
	render_target_view_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	render_target_view_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	render_target_view_desc.NodeMask = 0;
	

	result = device->CreateDescriptorHeap(&render_target_view_desc, __uuidof(ID3D12DescriptorHeap), (void**)(&render_target_view_descriptor_heap) );


	 descriptor_heap_size = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);



	 CD3DX12_CPU_DESCRIPTOR_HANDLE  rtv_handle(render_target_view_descriptor_heap->GetCPUDescriptorHandleForHeapStart());

	 for (UINT i = 0; i < 2; i++) {
		 swap_chain->GetBuffer(i, __uuidof(ID3D12Resource), (void**)(&render_targets[i]));
		 device->CreateRenderTargetView(render_targets[i].Get(), nullptr, rtv_handle);
		 rtv_handle.Offset(1, descriptor_heap_size);
	 }

	 result = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator) ,(void**)(&command_allocator));


	 create_root_signature();
	 create_pipeline_state_object();
	 create_syncronization_objects();
	 record_command();

}
void Renderer::create_root_signature() {

}

void Renderer::record_command() {
	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, command_allocator, nullptr, __uuidof(ID3D12GraphicsCommandList) , (void**)(&command_list));
	
	command_list->Close();


}
void Renderer::create_pipeline_state_object() {

}

void Renderer::create_syncronization_objects() {
	device->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), (void**)(&fence));
	fence_value = 1;

	fence_event = CreateEvent(nullptr, FALSE, FALSE, nullptr);

}

void Renderer::populate_command_list() {
	command_allocator->Reset();

	command_list->Reset(command_allocator, nullptr);

	command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(render_targets[frame_index].Get(),
										D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(render_target_view_descriptor_heap->GetCPUDescriptorHandleForHeapStart(),
											frame_index,
											descriptor_heap_size);

	command_list->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

	// Record commands.
	const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
	command_list->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

	command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(render_targets[frame_index].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	command_list->Close();


}

void Renderer::wait_for_previous_frame() {

}

void Renderer::draw_frame() {
	populate_command_list();

	ID3D12CommandList* ppCommandLists[] = {command_list};
	command_queue->ExecuteCommandLists(1, ppCommandLists);


	swap_chain->Present(1, 0);
	wait_for_previous_frame();
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