#pragma once
#include <Windows.h>
#include <d3d12.h>

#include <DXGI1_5.h>
#include <d3dcompiler.h>

#pragma comment(lib,"d3dcompiler.lib")

#include <wrl.h>
#include <vector>
using namespace Microsoft::WRL;

class Engine;
class Renderer
{
public:
	Engine* engine;

	ID3D12Device* device;
	ID3D12CommandQueue* command_queue;
	ComPtr<IDXGISwapChain3> swap_chain;
	ComPtr<ID3D12Resource> render_targets[2];
	ID3D12DescriptorHeap* render_target_view_descriptor_heap;
	ID3D12CommandAllocator* command_allocator;

	ID3D12RootSignature* root_signature;

	//Syncronization Objects
	ID3D12Fence* fence;
	HANDLE fence_event;
	UINT64 fence_value;
	UINT frame_index;

	UINT descriptor_heap_size;

	ID3DBlob* VS, * PS;
	ComPtr<ID3D12PipelineState> pipeline;

	ID3D12GraphicsCommandList* command_list;
	//std::vector<ID3D12Resource*> render_targets;
	void init();
	void get_hardware_adapter(IDXGIFactory4* pFactory, IDXGIAdapter1** ppAdapter);
	void create_pipeline_state_object();
	void create_root_signature();
	void record_command();
	void create_syncronization_objects();
	void populate_command_list();
	void wait_for_previous_frame();
	void draw_frame();
	void load_and_compile_shaders();
	HRESULT create_shader(LPCWSTR path, ID3DBlob** shader_blob, LPCSTR type, LPCSTR profile);
};

