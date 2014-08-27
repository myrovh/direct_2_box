#include <Windows.h>
#include <d3dx9.h>


class directx
{
public:
	directx(HINSTANCE instance);
	~directx();

	void run();
	bool create_directx();
	bool create_window();

	LPDIRECT3D9 direct3d_object = NULL;
	LPDIRECT3DDEVICE9 direct3d_device = NULL;

private:
	bool window_created;
	bool directx_created;
	HINSTANCE window_instance;
	HWND window_handler;
};

extern directx* engine_pointer;