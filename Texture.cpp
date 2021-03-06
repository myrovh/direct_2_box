#include "Texture.h"

Texture::Texture()
{
	reference_count = 0;
	texture_storage = NULL;
}

Texture::~Texture()
{
	release();
}

bool Texture::load(LPDIRECT3DDEVICE9 device, std::string filename)
{
	if(FAILED(D3DXCreateTextureFromFile(device, filename.c_str(), &texture_storage)))
	{
		return FALSE;
	}

	this->filename = filename;
	return TRUE;
}

void Texture::release()
{
	if(texture_storage != NULL)
	{
		texture_storage->Release();
		texture_storage = NULL;
	}
}