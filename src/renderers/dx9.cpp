#include "..//engine.h"
#include "dx9.hpp"

#define CUSTOMFVF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

void Renderer::init(){
	d3d = Direct3DCreate9(D3D_SDK_VERSION);    // create the Direct3D interface

	D3DPRESENT_PARAMETERS d3dpp;    // create a struct to hold various device information

	ZeroMemory(&d3dpp, sizeof(d3dpp));    // clear out the struct for use
	d3dpp.Windowed = TRUE;    // program windowed, not fullscreen
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;    // discard old frames
	d3dpp.hDeviceWindow = engine->window_manager.window_handler;    // set the window to be used by Direct3D

	// create a device class using this information and information from the d3dpp stuct
	HRESULT result = d3d->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		engine->window_manager.window_handler,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev);

	std::cout << "device created\n";
	

	Vertex vert1{};
	vert1.pos = glm::vec3(320.0f, 50.0f, 1.0f);
	vert1.texCoord = vec2(0, 1);

	Vertex vert2{};
	vert2.pos = glm::vec3(520.0f, 400.0f, 1.0f);
	vert2.texCoord = vec2(0, 0);

	Vertex vert3{};
	vert3.pos = glm::vec3(120.0f, 400.0f, 1.0f);
	vert3.texCoord = vec2(1, 1);

	Vertex vertices[] = { vert1,vert2,vert3 };

	d3ddev->CreateVertexBuffer(3 * sizeof(Vertex),
		0,
		CUSTOMFVF,
		D3DPOOL_MANAGED,
		&vertex_buffer,
		NULL);

	VOID* pVoid;    // the void* we were talking about

	vertex_buffer->Lock(0, 0, (void**)& pVoid, 0);    // locks v_buffer, the buffer we made earlier

	memcpy(pVoid, vertices, sizeof(vertices));    // copy vertices to the vertex buffer
	vertex_buffer->Unlock();


}

void Renderer::draw_frame() {
	// clear the window to a deep blue
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 40, 100), 1.0f, 0);

	d3ddev->BeginScene();    // begins the 3D scene

		 // select which vertex format we are using
		d3ddev->SetFVF(CUSTOMFVF);

		// select the vertex buffer to display
		d3ddev->SetStreamSource(0, vertex_buffer, 0, sizeof(Vertex));

		// copy the vertex buffer to the back buffer
		d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

	d3ddev->EndScene();    // ends the 3D scene

	d3ddev->Present(NULL, NULL, NULL, NULL);    // displays the created frame
}