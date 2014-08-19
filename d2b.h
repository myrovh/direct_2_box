#include <Windows.h>
#include <d3dx9.h>


class d2b
{
public:
	d2b(d2b_setup* d2b_settings);
	virtual ~d2b();

	d2b* engine_pointer = NULL;

	void d2b_run();
	HWND get_window_handel();

private:
	void d2b_render();

	bool directx_loaded;
	HWND window_handel;
	d2b_setup* default_settings = NULL;
};

struct d2b_setup
{
	HINSTANCE window_instance;
	char* window_name;

	d2b_setup()
	{
		window_instance = NULL;
		window_name = "Test Window";
	}
};

extern d2b* engine_pointer;