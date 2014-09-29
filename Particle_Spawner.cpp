#include "Particle_Spawner.h"

inline DWORD f_to_dw(FLOAT f) {
	return *((DWORD*)&f);
}

float get_random_min_max(float minimum, float maximum)
{
	float random_number = (float)rand() / RAND_MAX;
	return minimum + (maximum - minimum) * random_number;
}

D3DXVECTOR3 get_random_vector(void)
{
	D3DXVECTOR3 vector;

	//Pick a random Z between -1.0f and 1.0f
	vector.z = get_random_min_max(-1.0f, 1.0f);

	//Get radius of this circle
	float radius = (float)sqrt(1 - vector.z * vector.z);

	//Pick a random point on a circle
	float point = get_random_min_max(-D3DX_PI, D3DX_PI);

	//Compute matching x and y for z
	vector.x = (float)cosf(point) * radius;
	vector.y = (float)sinf(point) * radius;

	return vector;
}

Particle_Spawner::Particle_Spawner()
{
	buffer_offset = 0;
	chunk_size = 512;
	max_buffer_size = 2048;
	active_particle_list = NULL;
	free_particle_list = NULL;
	active_count = 0;
	current_time = 0.0f;
	last_update = 0.0f;
	vertex_buffer = NULL;
	particle_texture = NULL;
	max_particles = 1;
	number_to_release = 1;
	release_interval = 1.0f;
	particle_lifetime = 1.0f;
	particle_size = 1.0f;
	particle_colour = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	spawner_position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	system_velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	system_gravity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	system_wind = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	use_wind = FALSE;
	velocity_variance = 1.0f;
	lifetime_variance = 1.0f; //TODO implement this
	visible = FALSE;
}

Particle_Spawner::~Particle_Spawner()
{

}

bool Particle_Spawner::initialise(LPDIRECT3DDEVICE9 device)
{
	if(FAILED(device->CreateVertexBuffer(max_buffer_size * sizeof(Point_Vertex), (D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY | D3DUSAGE_POINTS), POINTFVF, D3DPOOL_DEFAULT, &vertex_buffer, NULL)))
	{
		return FALSE;
	}

	//Get max point size
	D3DCAPS9 d3d_caps;
	device->GetDeviceCaps(&d3d_caps);
	max_particle_size = d3d_caps.MaxPointSize;

	return TRUE;
}

void Particle_Spawner::release(Texture_Manager* texture_manage)
{
	if(vertex_buffer)
	{
		vertex_buffer->Release();
		vertex_buffer = NULL;
	}

	while(active_particle_list)
	{
		Particle* particle_pointer = active_particle_list;
		active_particle_list = particle_pointer->next_element;
		delete particle_pointer;
	}
	active_particle_list = NULL;

	while(free_particle_list)
	{
		Particle* particle_pointer = free_particle_list;
		free_particle_list = particle_pointer->next_element;
		delete particle_pointer;
	}
	free_particle_list = NULL;

	texture_manage->release_texture(particle_texture);
	particle_texture = NULL;
}

bool Particle_Spawner::update(float timestep)
{
	if(visible)
	{
		Particle* current_particle;
		Particle** particle_list_pointer;
		current_particle = new Particle();

		current_time += timestep;

		particle_list_pointer = &active_particle_list;

		//Update existing particles
		while(*particle_list_pointer)
		{
			current_particle = *particle_list_pointer;

			float time_passed = current_time - current_particle->created_time;

			if(time_passed >= particle_lifetime)
			{
				//Put particle on the free list
				*particle_list_pointer = current_particle->next_element;
				current_particle->next_element = free_particle_list;
				free_particle_list = current_particle;
			}
			else
			{
				//update velocity accounting for gravity
				current_particle->current_velocity += system_gravity * timestep;

				//Update velocity accounting for wind
				if(use_wind)
				{
					current_particle->current_velocity += (system_wind - current_particle->current_velocity) * timestep;

				}

				//Update position accounting for velocity
				current_particle->current_positon += current_particle->current_velocity * timestep;

				particle_list_pointer = &current_particle->next_element;
			}
		}

		//Create more particles according to flow rate
		if(current_time - last_update > release_interval)
		{
			//reset update timing
			last_update = current_time;

			//Emit new particles
			for(DWORD i = 0; i < number_to_release; ++i)
			{
				//If there are free particles use those to emit
				if(free_particle_list)
				{
					current_particle = free_particle_list;
					free_particle_list = current_particle->next_element;
				}
				else
				{
					if(active_count < max_particles)
					{
						current_particle = new Particle();
						if(!current_particle)
						{
							return FALSE;
						}
					}
				}

				if(active_count < max_particles)
				{
					current_particle->next_element = active_particle_list;
					active_particle_list = current_particle;

					current_particle->current_velocity = system_velocity;

					if(velocity_variance != 0.0f)
					{
						D3DXVECTOR3 random_vector = get_random_vector();
						current_particle->current_velocity += random_vector * velocity_variance;
					}

					current_particle->created_time = current_time;
					current_particle->current_positon = spawner_position;

					++active_count;
				}
			}
		}
	}

	return TRUE;
}

bool Particle_Spawner::render(LPDIRECT3DDEVICE9 device)
{
	if(visible)
	{
		device->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE); //TUrn on point sprites
		device->SetRenderState(D3DRS_POINTSCALEENABLE, TRUE); //Allow sprites to be scaled with distance
		device->SetRenderState(D3DRS_POINTSIZE, f_to_dw(particle_size)); //Float value that specifies the size to use for point size computation in cases where point size is not specified for each vertex
		device->SetRenderState(D3DRS_POINTSIZE_MIN, f_to_dw(1.0f)); //Float value that specified the minimum size of point primitives. Point primitives are clamped to this size during rendering
		device->SetRenderState(D3DRS_POINTSCALE_A, f_to_dw(0.0f)); //default 1.0
		device->SetRenderState(D3DRS_POINTSCALE_B, f_to_dw(0.0f)); //Default 0.0
		device->SetRenderState(D3DRS_POINTSCALE_C, f_to_dw(1.0f)); //Default 0.0

		//this resets the world matrix to stop the particle system following the last set world matrix (eg if this is disabled it will follow the player + offset
		D3DXMATRIX world;
		D3DXMatrixIdentity(&world);
		device->SetTransform(D3DTS_WORLD, &world);

		Particle* current_particle = active_particle_list;
		Point_Vertex* vertices;
		DWORD particles_to_render = 0;

		//Move offset forward so the next chunk can be filled
		buffer_offset += chunk_size;

		//If the buffer is about to overflow, reset counter back to 0
		overflow_reset();

		if(FAILED(vertex_buffer->Lock(buffer_offset * sizeof(Point_Vertex), chunk_size * sizeof(Point_Vertex), (void**)&vertices, buffer_offset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD)))
		{
			return FALSE;
		}

		//Render each particle
		while(current_particle)
		{
			vertices->position = current_particle->current_positon;
			vertices->colour = particle_colour;
			vertices++;

			if(++particles_to_render == chunk_size)
			{
				vertex_buffer->Unlock();

				device->SetStreamSource(0, vertex_buffer, 0, sizeof(Point_Vertex));
				device->SetFVF(POINTFVF);
				device->SetTexture(0, particle_texture->get_texture());
				if(FAILED(device->DrawPrimitive(D3DPT_POINTLIST, buffer_offset, particles_to_render)))
				{
					return FALSE;
				}

				buffer_offset += chunk_size;

				//If buffer is about to overflow reset
				overflow_reset();

				if(FAILED(vertex_buffer->Lock(buffer_offset * sizeof(Point_Vertex), chunk_size * sizeof(Point_Vertex), (void**)&vertices, buffer_offset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD)))
				{
					return FALSE;
				}

				particles_to_render = 0;
			}

			current_particle = current_particle->next_element;
		}

		vertex_buffer->Unlock();

		//Render any remaining particles
		if(particles_to_render)
		{
			device->SetStreamSource(0, vertex_buffer, 0, sizeof(Point_Vertex));
			device->SetFVF(POINTFVF);
			device->SetTexture(0, particle_texture->get_texture());
			if(FAILED(device->DrawPrimitive(D3DPT_POINTLIST, buffer_offset, particles_to_render)))
			{
				return FALSE;
			}
		}

		//Reset render states
		device->SetRenderState(D3DRS_POINTSCALEENABLE, FALSE);
		device->SetRenderState(D3DRS_POINTSPRITEENABLE, FALSE);
	}

	return TRUE;
}

void Particle_Spawner::overflow_reset()
{
	if(buffer_offset >= max_buffer_size)
	{
		buffer_offset = 0;
	}
}