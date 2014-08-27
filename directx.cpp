#include "directx.h"

directx* engine_pointer = NULL;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_KEYUP:
			// If the user presses the escape key then exit the application.
			if(wParam == VK_ESCAPE)
				PostQuitMessage(0);
			break;

		case WM_DESTROY:
		case WM_CLOSE:
			PostQuitMessage(0);
			break;

		default:
			break;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

directx::directx(HINSTANCE instance)
{
	window_instance = instance;
	engine_pointer = this;

	window_created = create_window();
	directx_created = create_directx();
}

bool directx::create_window() {
	// Define our window class.
	WNDCLASSEX myWindowClass;

	// WNDCLASSEX is an old C-style structure; it has no constructor, so it's a good
	// idea to set the structure's memory to 0 before poking around in it.
	memset(&myWindowClass, 0, sizeof(myWindowClass));

	myWindowClass.cbSize = sizeof(WNDCLASSEX); // Magic required by Windows.
	myWindowClass.style = CS_CLASSDC;
	// Here, we give the window class a pointer to our window procedure function...
	myWindowClass.lpfnWndProc = &WndProc;
	// ...and a handle to our program instance, conveniently passed as an argument to WinMain.
	myWindowClass.hInstance = window_instance;
	// Finally, name our class.
	myWindowClass.lpszClassName = "MyFirstWindowClass";

	// Register the class with Windows so we can use it.
	RegisterClassEx(&myWindowClass);

	// Now, create a window using our class.
	window_handler = CreateWindow(
		"MyFirstWindowClass",		// The class name to use.
		"My Window Title",
		WS_OVERLAPPEDWINDOW,		// Tell windows "I want a regular window".
		100, 100,					// x, y position, in pixels
		300, 300,					// x, y size, in pixels
		GetDesktopWindow(),			// Our parent window.
		NULL,
		myWindowClass.hInstance,	// Again, our instance handle.
		NULL
		);

	return true;
}

bool directx::create_directx()
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

void directx::run()
{
	ShowWindow(window_handler, SW_SHOW);

	Sleep(5000);

	DestroyWindow(window_handler);
	UnregisterClass("MyFirstWindowClass", window_instance);
}