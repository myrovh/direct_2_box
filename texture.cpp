#include "texture.h"

texture::texture()
{
	reference_count = 0;
	texture_storage = NULL;
	filename = NULL;
}


texture::~texture()
{
	release();
}

bool texture::load(LPDIRECT3DDEVICE9 device, const char* filename)
{
	if(FAILED(D3DXCreateTextureFromFile(device, filename, &texture_storage)))
	{
		return FALSE;
	}

	this->filename = filename;
	return TRUE;
}

void texture::release()
{
	if(texture_storage != NULL)
	{
		texture_storage->Release();
		texture_storage = NULL;
	}
}