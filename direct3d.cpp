#include "direct3d.h"

direct3d::direct3d()
{
	direct3d_device = NULL;
	direct3d_object = NULL;
}

direct3d::~direct3d()
{
	release();
}

bool direct3d::initialise(HWND window_handler, bool fullscreen)
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

	present_parameters.Windowed = TRUE;
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

	return TRUE;
}

void direct3d::render(std::vector<object*> object_queue, camera_fixed* camera)
{
	direct3d_device->SetTransform(D3DTS_VIEW, &camera->get_view());
	direct3d_device->SetTransform(D3DTS_PROJECTION, &camera->get_projection());

	// Clear the screen to black.
	direct3d_device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
						D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	// Tell Direct 3D to start drawing.
	if(SUCCEEDED(direct3d_device->BeginScene()))
	{
		for(int i = 0; i < object_queue.size(); i++)
		{
			object_queue[i]->render(direct3d_device);
		}
		direct3d_device->EndScene();
	}
	// Swap the old frame with the new one.
	direct3d_device->Present(NULL, NULL, NULL, NULL);
}

void direct3d::release()
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