#ifndef TEXTURE_H
#define TEXTURE_H
#include <d3dx9.h>

class texture
{
private:
	int reference_count;
	LPDIRECT3DTEXTURE9 texture_storage;
	const char* filename;

	void release();

protected:
	texture();
	~texture();
	void add_reference() {
		reference_count++;
	}
	void remove_reference() {
		reference_count--;
	}
	int get_reference_count() {
		return reference_count;
	}

public:
	bool load(LPDIRECT3DDEVICE9 device, const char* filename);
	LPDIRECT3DTEXTURE9 get_texture() {
		return texture_storage;
	}
	const char* get_filename() {
		return filename;
	}

	friend class texture_manager;
};

#endif