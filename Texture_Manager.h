#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <d3dx9.h>
#include <map>
#include <string>

#include "Texture.h"

class Texture_Manager
{
private:
	typedef std::map<std::string, Texture*> texture_map;
	texture_map texture_queue;

public:
	Texture_Manager();
	~Texture_Manager();
	bool load(LPDIRECT3DDEVICE9 device, const char* filename);
	Texture* get_texture(const char* filename);
	void release_texture(Texture* texture);
	void release();
};

#endif
