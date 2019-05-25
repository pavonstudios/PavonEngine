#include "../engine.h"
#include "directx.hpp"

#include <DirectXMath.h>

using namespace DirectX;

void Renderer::init(){
	DXGI_SWAP_CHAIN_DESC scd;
	
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));


	// fill the swap chain description struct
	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = GetActiveWindow();// the window to be used
	scd.SampleDesc.Count = 4;                               // how many multisamples
	scd.Windowed = TRUE;                                    // windowed/full-screen mode

	// create a device, device context and swap chain using the information in the scd struct
	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&swapchain,
		&dev,
		NULL,
		&devcon);

	std::cout << "Device and Swap chain created\n";

	// get the address of the back buffer
	ID3D11Texture2D* pBackBuffer = nullptr;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& pBackBuffer);

	// use the back buffer address to create the render target
	dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
	pBackBuffer->Release();

	// set the render target as the back buffer
	devcon->OMSetRenderTargets(1, &backbuffer, NULL);


	// Set the viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = 800;
	viewport.Height = 600;

	devcon->RSSetViewports(1, &viewport);


	//Describe our Depth/Stencil Buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc;


	depthStencilDesc.Width = 800;
	depthStencilDesc.Height = 600;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;


	//Create the Depth/Stencil View
	dev->CreateTexture2D(&depthStencilDesc, NULL, &depthStencilBuffer);
	dev->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView);

	//Set our Render Target
	devcon->OMSetRenderTargets(1, &backbuffer, depthStencilView);

	init_pipeline();


	mesh = new EMesh;
	engine->mesh_manager.load_model_gltf(mesh, "C:\\MinGW\\msys\\1.0\\home\\pavon\\PavonEngine\\Game\\Assets\\models\\pavon_the_game\\lince.gltf");

	create_mesh_buffers(mesh);
}

void Renderer::draw_frame() {
	// clear the back buffer 
	float color[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	devcon->ClearRenderTargetView(backbuffer, color);

	update_constant_buffer(); 
	
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	
	devcon->IASetVertexBuffers(0, 1, &mesh->vertex_buffer, &stride, &offset);
	devcon->IASetIndexBuffer(mesh->indices_buffer, DXGI_FORMAT_R32_UINT, 0);


	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	devcon->Draw(mesh->vertices.size(), 0);    

	devcon->DrawIndexed(mesh->indices.size(), 0, 0);

	
	
	//devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);

	//devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//devcon->Draw(3, 0);

	swapchain->Present(0, 0);
}

void Renderer::update_constant_buffer()
{

	glm::mat4 mat1 = glm::mat4(1.0);

	XMMATRIX mat2;
	
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(UniformBufferObject);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;

	if (FAILED(dev->CreateBuffer(&bd, NULL, &uniform_buffer))) {
		throw std::runtime_error("Pixel Shader compiler ERROR");
		std::cout << "error in created uniform buffer\n";
	}
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	XMMATRIX model = XMMatrixIdentity();
	XMVECTOR rot_vector = XMVectorSet(0, 0, 1, 0);
	model = XMMatrixRotationAxis(rot_vector , time * 0.5);
	XMVECTOR eye = XMVectorSet(0, 5.f, 0.f, 0);
	XMVECTOR postion = XMVectorSet(0, 0, 0, 0);
	XMVECTOR up = XMVectorSet(0, 0, 1, 0);

	XMMATRIX view = XMMatrixLookAtRH(eye, postion, up);


	XMMATRIX proj = XMMatrixPerspectiveFovRH(45.f, 800.f / 600.f, 0.001f, 10000.f);

	UniformBufferObject ubo = {};
	ubo.model = XMMatrixTranspose( model );
	ubo.view = XMMatrixTranspose( view );
	ubo.projection = XMMatrixTranspose(proj);

	

	devcon->VSSetConstantBuffers(0, 1, &uniform_buffer);
	devcon->UpdateSubresource(uniform_buffer, 0, NULL, &ubo, 0, 0);

	uniform_buffer->Release();

}

void Renderer::init_pipeline() {

	ID3D10Blob* VS, * PS;

	HRESULT vertext_result =  create_shader(L"C:\\Users\\pavon\\source\\repos\\direct\\Debug\\vertex.hlsl", &VS, "vs_5_0","vs_4_0_level_9_1" );
	if (FAILED(vertext_result)) {
		throw std::runtime_error("Vertex shader compile ERROR");
		return;

	}
	HRESULT pixel_result = create_shader(L"C:\\Users\\pavon\\source\\repos\\direct\\Debug\\pixel.hlsl", &PS, "PS", "ps_4_0_level_9_1");

	if (FAILED(pixel_result)) {
		throw std::runtime_error("Pixel Shader compiler ERROR");
	}

	ID3D11VertexShader* vertex_shader;
	ID3D11PixelShader* pixel_shader;

	 dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &vertex_shader);
	

	 dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pixel_shader);
	
	devcon->VSSetShader(vertex_shader, 0, 0);
	devcon->PSSetShader(pixel_shader, 0, 0);

	

	create_buffer(&pVBuffer);

	float multiplies = 5;
	Vertex vert1{};
	vert1.pos = glm::vec3(0.0, 0.5* multiplies, 0.0);
	

	Vertex vert2{};
	vert2.pos = glm::vec3(0.45* multiplies, -0.5* multiplies, 0.0);
	

	Vertex vert3{};
	vert3.pos = glm::vec3(-0.45* multiplies, -0.5* multiplies, 0.0);

	Vertex vertices[] = { vert1,vert2,vert3 };

	std::vector<Vertex> vertexs;
	vertexs.push_back(vert1);
	vertexs.push_back(vert2);
	vertexs.push_back(vert3);

	// copy the vertices into the buffer
	D3D11_MAPPED_SUBRESOURCE ms;
	devcon->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
	memcpy(ms.pData, vertexs.data(), sizeof(Vertex) * vertexs.size() );                 // copy the data
	devcon->Unmap(pVBuffer, NULL);                                      // unmap the buffer


	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
		//{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};


	dev->CreateInputLayout(ied, 1, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);
	devcon->IASetInputLayout(pLayout);


	


}

void Renderer::create_mesh_buffers(EMesh* mesh)
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;               
	bd.ByteWidth = sizeof(Vertex) * mesh->vertices.size();            
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;      
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;   

	dev->CreateBuffer(&bd, NULL, &mesh->vertex_buffer);      

	D3D11_MAPPED_SUBRESOURCE ms;
	devcon->Map(mesh->vertex_buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    
	memcpy(ms.pData, mesh->vertices.data(), sizeof(Vertex) * mesh->vertices.size() );
	devcon->Unmap(mesh->vertex_buffer, NULL);

	bd.ByteWidth = sizeof(unsigned int) * mesh->indices.size();
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	dev->CreateBuffer(&bd, NULL, &mesh->indices_buffer);

	devcon->Map(mesh->indices_buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, mesh->indices.data(), sizeof(unsigned) * mesh->indices.size());
	devcon->Unmap(mesh->indices_buffer, NULL);



}

void Renderer::create_buffer(ID3D11Buffer** buffer)
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	bd.ByteWidth = sizeof(Vertex) * 3;             // size is the VERTEX struct * 3
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

	dev->CreateBuffer(&bd, NULL, buffer);       // create the buffer
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
