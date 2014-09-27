#include "Renderer.h"

Renderer::Renderer()
{
	direct3d_device = NULL;
	direct3d_object = NULL;
	direct3d_font = NULL;
	width = 0;
	height = 0;
}

Renderer::~Renderer()
{
	release();
}

bool Renderer::initialise(HWND window_handler, UINT width, UINT height, bool fullscreen)
{
	D3DDISPLAYMODE DisplayMode;
	D3DPRESENT_PARAMETERS present_parameters;
	D3DCAPS9 D3DCaps;
	ZeroMemory(&present_parameters, sizeof(present_parameters));
	direct3d_object = Direct3DCreate9(D3D_SDK_VERSION);

	if(direct3d_object == NULL)
	{
		MessageBox(NULL, "Error, couldn't initialize DirectX!?!",
				   "Error!", MB_OK);
		return FALSE;
	}

	if(FAILED(direct3d_object->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &DisplayMode)))
	{
		MessageBox(NULL, "Error setting the display mode.", "Error!", MB_OK);
		return FALSE;
	}

	if(FAILED(direct3d_object->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &D3DCaps)))
	{
		return FALSE;
	}

	DWORD VertexProcessing = 0;
	if(D3DCaps.VertexProcessingCaps != 0)
	{
		VertexProcessing |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		VertexProcessing |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	if(fullscreen)
	{
		//not implemented
	}
	else
	{
		this->width = width;
		this->height = height;
		present_parameters.Windowed = TRUE;
	}
	present_parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	present_parameters.BackBufferFormat = DisplayMode.Format;
	present_parameters.BackBufferCount = 1;
	present_parameters.EnableAutoDepthStencil = TRUE;
	present_parameters.AutoDepthStencilFormat = D3DFMT_D16;

	if(FAILED(direct3d_object->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, window_handler,
		VertexProcessing,
		&present_parameters, &direct3d_device)))
	{
		MessageBox(NULL, "CreateDevice() failed!  Make sure you have DirectX 9.",
				   "Error!", MB_OK);
		return FALSE;
	}

	if(direct3d_device == NULL)
	{
		MessageBox(NULL, "D3D_Device is equal to NULL!?!", "Error!", MB_OK);
		return FALSE;
	}

	direct3d_device->SetRenderState(D3DRS_LIGHTING, FALSE);
	direct3d_device->SetRenderState(D3DRS_ZENABLE, TRUE);
	direct3d_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	direct3d_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	if(FAILED(D3DXCreateFont(
		direct3d_device,
		24,
		0,
		0,
		1,
		0,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		"Arial",
		&direct3d_font
		)))
	{
		return FALSE;
	}

	return TRUE;
}

void Renderer::render(std::vector<Object*> object_queue,
					  std::vector<Particle_Spawner*> particle_queue,
					  std::vector<Button*> button_queue,
					  std::vector<Text*> font_queue, Camera* camera)
{
	direct3d_device->SetTransform(D3DTS_VIEW, &camera->get_view());
	direct3d_device->SetTransform(D3DTS_PROJECTION, &camera->get_projection());

	// Clear the screen to black.
	direct3d_device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
						   D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	// Tell Direct 3D to start drawing.
	if(SUCCEEDED(direct3d_device->BeginScene()))
	{
		for(size_t i = 0; i < object_queue.size(); i++)
		{
			object_queue[i]->render(direct3d_device);
		}

		for(size_t i = 0; i < font_queue.size(); i++)
		{
			font_queue[i]->render(direct3d_font);
		}

		for(size_t i = 0; i < button_queue.size(); i++)
		{
			button_queue[i]->render();
		}

		//Adjust render state before rendering particles
		//TODO this is not an optimal method of setting render state might
		direct3d_device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		direct3d_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		direct3d_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		for(size_t i = 0; i < particle_queue.size(); i++)
		{
			particle_queue[i]->render(direct3d_device);
		}

		//Set render state back to normal
		direct3d_device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		direct3d_device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

		direct3d_device->EndScene();
	}
	// Swap the old frame with the new one.
	direct3d_device->Present(NULL, NULL, NULL, NULL);
}

void Renderer::release()
{
	if(direct3d_device)
	{
		direct3d_device->Release();
		direct3d_device = NULL;
	}

	if(direct3d_object)
	{
		direct3d_object->Release();
		direct3d_object = NULL;
	}
}