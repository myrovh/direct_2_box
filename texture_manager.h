#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <d3dx9.h>
#include <map>
#include <string>
#include "texture.h"

class texture_manager
{
private:
	typedef std::map<std::string, texture*> texture_map;
	texture_map texture_queue;

public:
	texture_manager();
	~texture_manager();
	bool load(LPDIRECT3DDEVICE9 device, const char* filename);
	texture* get_texture(const char* filename);
	void release_texture(texture* texture);
	void release();
};

#endif