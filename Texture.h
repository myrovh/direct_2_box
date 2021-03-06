#ifndef TEXTURE_H
#define TEXTURE_H
#include <d3dx9.h>
#include <string>

class Texture
{
private:
	int reference_count;
	LPDIRECT3DTEXTURE9 texture_storage;
	std::string filename;

	void release();

protected:
	Texture();
	~Texture();
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
	bool load(LPDIRECT3DDEVICE9 device, std::string filename);
	LPDIRECT3DTEXTURE9 get_texture() {
		return texture_storage;
	}
	std::string get_filename() {
		return filename;
	}

	friend class Texture_Manager;
};

#endif
