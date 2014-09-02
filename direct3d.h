#ifndef DIRECT3D_H
#define DIRECT3D_H

#include <d3dx9.h>
#include <vector>
#include "object.h"
#include "camera_fixed.h"

class direct3d
{
private:
	LPDIRECT3D9 direct3d_object;
	LPDIRECT3DDEVICE9 direct3d_device;

public:
	direct3d();
	~direct3d();

	bool initialise(HWND window_instance, bool fullscreen);
	LPDIRECT3DDEVICE9 get_device() { return direct3d_device; }
	void render(std::vector<object*> object_queue, camera_fixed* camera);
	void release();
};

#endif