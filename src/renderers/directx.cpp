#include "../engine.h"
#include "directx.hpp"

#include <DirectXMath.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace DirectX;

void Renderer::init(){
	DXGI_SWAP_CHAIN_DESC scd;
	
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));


	scd.BufferCount = 2;                                  
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;    
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      
	scd.OutputWindow = GetActiveWindow();
	scd.SampleDesc.Count = 1;                             
	scd.Windowed = TRUE;                                   
	

	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&swapchain,
		&dev,
		NULL,
		&devcon);

	std::cout << "Device and Swap chain created\n";


	ID3D11Texture2D* pBackBuffer = nullptr;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& pBackBuffer);



	dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
	

	D3D11_TEXTURE2D_DESC depthStencil_texture;


	depthStencil_texture.Width = 800;
	depthStencil_texture.Height = 600;
	depthStencil_texture.MipLevels = 1;
	depthStencil_texture.ArraySize = 1;
	depthStencil_texture.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencil_texture.SampleDesc.Count = 1;
	depthStencil_texture.SampleDesc.Quality = 0;
	depthStencil_texture.Usage = D3D11_USAGE_DEFAULT;
	depthStencil_texture.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencil_texture.CPUAccessFlags = 0;
	depthStencil_texture.MiscFlags = 0;

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(dsvDesc));
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

	HRESULT result;
	 result = dev->CreateTexture2D(&depthStencil_texture, NULL, &depthStencilBuffer);
	result = dev->CreateDepthStencilView(depthStencilBuffer, &dsvDesc, &depthStencilView);




	devcon->OMSetRenderTargets(1, &backbuffer, depthStencilView);
	pBackBuffer->Release();


	D3D11_DEPTH_STENCIL_DESC depth_stencil_desc;
	ZeroMemory(&depth_stencil_desc, sizeof(depth_stencil_desc));
	depth_stencil_desc.DepthEnable = true;
	depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	depth_stencil_desc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

	result = dev->CreateDepthStencilState(&depth_stencil_desc, &pDepthStencilState);

	devcon->OMSetDepthStencilState(pDepthStencilState, 0);
	

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = 800;
	viewport.Height = 600;
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;

	devcon->RSSetViewports(1, &viewport);


	init_pipeline();


	mesh = new EMesh;
	engine->mesh_manager.load_model_gltf(mesh, ".\\Game\\Assets\\models\\pavon_the_game\\lince.gltf");
	create_mesh_buffers(mesh);

	for (EMesh* mesh : engine->meshes) {
		create_mesh_buffers(mesh);
		load_texture(mesh);
	}
	
	

	//engine->meshes.push_back(mesh);
	
}

void Renderer::draw_frame() {
	// clear the back buffer 
	float color[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	devcon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	devcon->ClearRenderTargetView(backbuffer, color);
	
	

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	for (EMesh* mesh : engine->meshes) {
		update_constant_buffer(mesh);

		devcon->PSSetShaderResources(0, 1, &mesh->shader_resource_view);
		devcon->PSSetSamplers(0, 1, &mesh->sampler_state);


		

		devcon->IASetVertexBuffers(0, 1, &mesh->vertex_buffer, &stride, &offset);
		devcon->IASetIndexBuffer(mesh->indices_buffer, DXGI_FORMAT_R32_UINT, 0);


		devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);



		devcon->DrawIndexed(mesh->indices.size(), 0, 0);
	}
	
	
	devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);

	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	devcon->Draw(3, 0);

	swapchain->Present(0, 0);
}

void Renderer::update_constant_buffer(EMesh* mesh)
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

#ifdef  XM
	XMMATRIX model = XMMatrixIdentity();
	XMVECTOR rot_vector = XMVectorSet(0, 0, 1, 0);
	model = XMMatrixRotationAxis(rot_vector, time * 0.5);
	XMVECTOR eye = XMVectorSet(0, 4.f, 2.f, 0);
	XMVECTOR postion = XMVectorSet(0, 0, 0, 0);
	XMVECTOR up = XMVectorSet(0, 0, 1, 0);

	XMMATRIX view = XMMatrixLookAtLH(eye, postion, up);


	XMMATRIX proj = XMMatrixPerspectiveFovLH(45.f, 800.f / 600.f, 0.001f, 10000.f);
#endif //  XM

	glm::mat4 model = glm::rotate(glm::mat4(1.0), glm::radians(time * 30.f), glm::vec3(0, 0, 1));
	model = mesh->model_matrix;
	glm::mat4 view = glm::lookAtLH(glm::vec3(0.f, 4.f, 2.f), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
	glm::mat4 proj = glm::perspectiveLH(45.f, 800.f / 600.f, 0.001f, 10000.f);

	proj = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, 0.5f)) * glm::scale(glm::mat4(1.0), glm::vec3(1.0f, 1.0f, 0.5f)) * proj;

	

	UniformBufferObject ubo = {};
#ifdef XM

	ubo.model = XMMatrixTranspose(model);
	ubo.view = XMMatrixTranspose(view);
	ubo.projection = XMMatrixTranspose(proj);
#endif // XM
	ubo.model = model;
	ubo.view = view;
	ubo.proj = proj;



	

	devcon->VSSetConstantBuffers(0, 1, &uniform_buffer);
	devcon->UpdateSubresource(uniform_buffer, 0, NULL, &ubo, 0, 0);

	uniform_buffer->Release();

}

void Renderer::init_pipeline() {

	ID3D10Blob* VS, * PS;

	HRESULT vertext_result =  create_shader(L".\\Game\\Assets\\shaders\\dx\\vertex.hlsl", &VS, "vs_tex","vs_4_0_level_9_1" );
	if (FAILED(vertext_result)) {
		throw std::runtime_error("Vertex shader compile ERROR");
		return;

	}
	HRESULT pixel_result = create_shader(L".\\Game\\Assets\\shaders\\dx\\pixel.hlsl", &PS, "PS", "ps_4_0_level_9_1");

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
		{"SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, pos), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(Vertex, texCoord), D3D11_INPUT_PER_VERTEX_DATA, 0},

	};

	uint number_of_element = ARRAYSIZE(ied);
	dev->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);
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

void Renderer::load_texture(EMesh* mesh)
{
	D3D11_TEXTURE2D_DESC desc;
	desc.Width = 1024;
	desc.Height = 1024;
	desc.MipLevels = desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0.1;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;

	///Image image = engine->assets.load_and_get_size(".\\Game\\Assets\\textures\\pavon_the_game\\lince.png");
	Image image = mesh->image;

	D3D11_SUBRESOURCE_DATA init_data;
	init_data.pSysMem = image.pPixels;
	init_data.SysMemPitch = (UINT)(image.width * 4);
	init_data.SysMemSlicePitch = (UINT)(image.width * image.heigth * 4);
	HRESULT hr = dev->CreateTexture2D(&desc, &init_data, &mesh->dx_texture);
	if (FAILED(hr))
	{
		throw std::exception();
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	memset(&SRVDesc, 0, sizeof(SRVDesc));
	SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = 1;

	dev->CreateShaderResourceView(mesh->dx_texture, &SRVDesc, &mesh->shader_resource_view);

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	dev->CreateSamplerState(&sampDesc, &mesh->sampler_state);



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
