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
	CD3DX12_ROOT_SIGNATURE_DESC root_signature_desc;
	root_signature_desc.Init(0, nullptr, 0, nullptr,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
	);

	ComPtr<ID3DBlob> signature;
	ComPtr<ID3DBlob> error;

	D3D12SerializeRootSignature(&root_signature_desc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
	device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), __uuidof(ID3D12RootSignature) , (void**)(&root_signature));



}


HRESULT Renderer::create_shader(LPCWSTR path, ID3DBlob** shader_blob, LPCSTR type, LPCSTR profile)
{

#define DEBUG
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	flags |= D3DCOMPILE_DEBUG;
#endif
	// Prefer higher CS shader profile when possible as CS 5.0 provides better performance on 11-class hardware.
	//LPCSTR profile = (dev->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0) ? "cs_5_0" : "cs_4_0";
	const D3D_SHADER_MACRO defines[] =
	{
		"EXAMPLE_DEFINE", "1",
		NULL, NULL
	};

	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3DCompileFromFile(path, defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		type, profile,
		flags, 0, shader_blob, &errorBlob);

	if (FAILED(hr))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());

			const char* errorMsg = (const char*)errorBlob->GetBufferPointer();
			MessageBox(nullptr, errorMsg, "Shader Compilation Error", MB_RETRYCANCEL);
			errorBlob->Release();
		}

		return hr;
	}


	return hr;
}

void Renderer::record_command() {
	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, command_allocator, nullptr, __uuidof(ID3D12GraphicsCommandList) , (void**)(&command_list));
	
	command_list->Close();


}
void Renderer::create_pipeline_state_object() {
	load_and_compile_shaders();

	D3D12_INPUT_ELEMENT_DESC input_element_desc[] = {
		{"SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};
	D3D12_SHADER_BYTECODE vs_shader_byte_code;
	vs_shader_byte_code.BytecodeLength = VS->GetBufferSize();
	vs_shader_byte_code.pShaderBytecode = reinterpret_cast<UINT8*>(VS->GetBufferPointer());

	D3D12_SHADER_BYTECODE ps_shader_byte_code;
	ps_shader_byte_code.BytecodeLength = PS->GetBufferSize();
	ps_shader_byte_code.pShaderBytecode = reinterpret_cast<UINT8*>(PS->GetBufferPointer());

	D3D12_DEPTH_STENCIL_DESC depth_stencil_desc;
	depth_stencil_desc.DepthEnable = false;
	depth_stencil_desc.StencilEnable = false;
	depth_stencil_desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	depth_stencil_desc.DepthFunc = D3D12_COMPARISON_FUNC_NEVER;
	
	D3D12_INPUT_LAYOUT_DESC input_layout_desc;
	input_layout_desc.NumElements = 1;
	input_layout_desc.pInputElementDescs = input_element_desc;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipeline_state_desc;
	pipeline_state_desc.InputLayout = input_layout_desc;

	pipeline_state_desc.pRootSignature = root_signature;
	pipeline_state_desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	pipeline_state_desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	pipeline_state_desc.DepthStencilState = depth_stencil_desc;
	pipeline_state_desc.SampleMask = UINT_MAX;
	pipeline_state_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	pipeline_state_desc.NumRenderTargets = 1;
	pipeline_state_desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	pipeline_state_desc.SampleDesc.Count = 1;
	

	pipeline_state_desc.VS = vs_shader_byte_code;
	pipeline_state_desc.PS = ps_shader_byte_code;

	HRESULT hr = device->CreateGraphicsPipelineState(&pipeline_state_desc, __uuidof(ID3D12PipelineState), (void**)(&pipeline));


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

void Renderer::load_and_compile_shaders()
{
	

	HRESULT vertext_result = create_shader(L".\\Game\\Assets\\shaders\\dx\\simple_vertex.hlsl", &VS, "VS", "vs_5_0");
	if (FAILED(vertext_result)) {
		throw std::runtime_error("Vertex shader compile ERROR");
		return;

	}
	HRESULT pixel_result = create_shader(L".\\Game\\Assets\\shaders\\dx\\simple_pixel_shader.hlsl", &PS, "simple_ps", "ps_5_0");

	if (FAILED(pixel_result)) {
		throw std::runtime_error("Pixel Shader compiler ERROR");
	}



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