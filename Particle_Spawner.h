#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H
#include <d3dx9.h>
#include "Texture_Manager.h"

#define POINTFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)

struct Point_Vertex
{
	D3DXVECTOR3 position;
	D3DCOLOR colour;
};

struct Particle
{
	D3DXVECTOR3 current_positon;
	D3DXVECTOR3 current_velocity;
	float created_time;

	Particle* next_element;
};

class Particle_Spawner
{
private:
	Particle* active_particle_list; //Head node of point sprites that are active
	Particle* free_particle_list; //Head node of point sprites that are inactive and waiting to be recycled

	DWORD buffer_offset; //Gives the offset of the vertex buffer chunk that's currently being filled
	DWORD chunk_size; //Number of point sprites to load before sending them to hardware (512 = 2048 divided into 4 chunks)
	DWORD max_buffer_size; //Max number of point sprites the vertex buffer can load until we are forced to discard and start over

	DWORD active_count;
	float current_time;
	float last_update;
	bool visible;

	float max_particle_size;

	LPDIRECT3DVERTEXBUFFER9 vertex_buffer; //Vertex buffer where point sprites are to be stored
	Texture* particle_texture;

	DWORD max_particles;
	DWORD number_to_release;
	float release_interval;
	float particle_lifetime;
	float particle_size;
	D3DXCOLOR particle_colour;
	D3DXVECTOR3 spawner_position;
	D3DXVECTOR3 system_velocity;
	D3DXVECTOR3 system_gravity;
	D3DXVECTOR3 system_wind;
	bool use_wind;
	float velocity_variance;
	float lifetime_variance;

	void overflow_reset();

public:
	Particle_Spawner();
	~Particle_Spawner();

	void set_max_particles(int max_particles) {
		this->max_particles = max_particles;
	}
	void set_number_to_release(int number_to_release) {
		this->number_to_release;
	}
	void set_release_interval(int release_interval) {
		this->release_interval = release_interval;
	}
	void set_lifetime(float lifetime) {
		this->particle_lifetime = lifetime;
	}
	void set_size(float size) {
		this->particle_size = size;
	}
	void set_particle_colour(D3DXCOLOR colour) {
		this->particle_colour = colour;
	}
	void set_position(D3DXVECTOR3 position) {
		this->spawner_position = position;
	}
	void set_velocity(D3DXVECTOR3 velocity) {
		this->system_velocity = velocity;
	}
	void set_gravity(D3DXVECTOR3 gravity) {
		this->system_gravity = gravity;
	}
	void set_wind(D3DXVECTOR3 wind) {
		this->system_wind = wind;
	}
	void set_use_wind(bool use_wind) {
		this->use_wind = use_wind;
	}
	void set_velocity_variance(float variance) {
		this->velocity_variance = variance;
	}
	void set_lifetime_variance(float variance) {
		this->lifetime_variance = variance;
	}
	void set_particle_texture(Texture* texture) {
		this->particle_texture = texture;
	}
	void set_visible(){
		visible = TRUE;
	}
	void set_invisible(){
		visible = FALSE;
	}

	bool is_visible(){
		return visible;
	}

	bool initialise(LPDIRECT3DDEVICE9 device);
	bool update(float timestep);
	bool render(LPDIRECT3DDEVICE9 device);
	void release(Texture_Manager* texture_manage);
};

#endif