#include "direct3d.h"

direct3d::direct3d()
{
	direct3d_device = NULL;
	direct3d_object = NULL;
}

direct3d::~direct3d()
{
	if(direct3d_device != NULL)
	{
		direct3d_device->Release();
		direct3d_device = NULL;
	}

	if(direct3d_object != NULL)
	{
		direct3d_object->Release();
		direct3d_object = NULL;
	}
}

bool direct3d::initialise(HWND window_handler, bool fullscreen)
{
	bool valid = false;

	// This object will allow us to set the display mode of the screen.
	D3DDISPLAYMODE DisplayMode;

	// This will allow us to set the parameters of the screen.
	D3DPRESENT_PARAMETERS Present_Parameters;

	// This is used to get the capabilities of the hardware.
	D3DCAPS9 D3DCaps;

	// It is always a good idea to clear out memory in object although not necessary.
	ZeroMemory(&Present_Parameters, sizeof(Present_Parameters));

	// Create the Direct3D object to get everything started.
	direct3d_object = Direct3DCreate9(D3D_SDK_VERSION);

	// Error checking.  Make sure that it was successful.
	if(direct3d_object == NULL)
	{
		MessageBox(NULL, "Error, couldn't initialize DirectX!?!",
				   "Error!", MB_OK);
		return false;
	}

	// This function will get the display mode of the device and place it in DisplayMode.
	if(FAILED(direct3d_object->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &DisplayMode)))
	{
		MessageBox(NULL, "Error setting the display mode.", "Error!", MB_OK);
		return false;
	}

	// Get the capabilities of the hardware.
	if(FAILED(direct3d_object->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &D3DCaps)))
	{
		return false;
	}

	// Test which is supported, hardware or software vertex processing.
	DWORD VertexProcessing = 0;

	if(D3DCaps.VertexProcessingCaps != 0)
		VertexProcessing |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		VertexProcessing |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	Present_Parameters.Windowed = TRUE;                   // Window mode (not fullscreen)
	Present_Parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;   // Dealing with animation (see doc).
	Present_Parameters.BackBufferFormat = DisplayMode.Format;// Render to the area of the screen.
	Present_Parameters.BackBufferCount = 1;                  // Number of back buffers.
	Present_Parameters.EnableAutoDepthStencil = TRUE;        // Check documentation.
	Present_Parameters.AutoDepthStencilFormat = D3DFMT_D16;  // Check documentation.


	// Now we must create the rendering device.
	if(FAILED(direct3d_object->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, window_handler,
		VertexProcessing,
		&Present_Parameters, &direct3d_device)))
	{
		MessageBox(NULL, "CreateDevice() failed!  Make sure you have DirectX 9.",
				   "Error!", MB_OK);
		return false;
	}


	// One last check to be sure.
	if(direct3d_device == NULL)
	{
		MessageBox(NULL, "D3D_Device is equal to NULL!?!", "Error!", MB_OK);
		return false;
	}

	// SetRenderState() will set a parameter for the rendering.  Here we turn off lighting.
	direct3d_device->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Disable culling.  This will render the front and back of the triangles.
	//D3D_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	direct3d_device->SetRenderState(D3DRS_ZENABLE, TRUE);

	// Set the texture min/mag info.
	direct3d_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	direct3d_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	return true;

	return valid;
}

void direct3d::render()
{

}