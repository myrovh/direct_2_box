#include "Object.h"

Object::Object()
{
	entity_mesh = NULL;
	scale_factor = 1.0f;
	entity_type = "none";
	axis_rotation = cos(0/2);
	hit_box = NULL;
}
Object::Object(Mesh* mesh, variable_map* constructor_settings, variable_map* variable_settings)
{
	//Assign dynamic variable pointer
	this->variable_settings = variable_settings;
	entity_type = boost::any_cast<std::string>(constructor_settings->at("entity_type"));

	//Assign mesh
	entity_mesh = mesh;
	scale_factor = boost::any_cast<float>(constructor_settings->at("scale"));

	//Set starting position
	vector_position.x = boost::any_cast<float>(constructor_settings->at("vec_x"));
	vector_position.y = boost::any_cast<float>(constructor_settings->at("vec_y"));
	vector_position.z = boost::any_cast<float>(constructor_settings->at("vec_z"));

	//Set rotations at zero
	D3DXQuaternionIdentity(&rotation);
	axis_rotation = cos(0/2);

	//Build Hit Sphere
	D3DXVECTOR3 hit_offset;
	hit_offset.x = boost::any_cast<float>(constructor_settings->at("hit_offset_x"));
	hit_offset.y = boost::any_cast<float>(constructor_settings->at("hit_offset_y"));
	hit_offset.z = boost::any_cast<float>(constructor_settings->at("hit_offset_z"));
	float hit_radius = boost::any_cast<float>(constructor_settings->at("hit_radius"));
	this->hit_box = new Collision_Sphere(hit_offset, hit_radius);
}

Object::~Object()
{
}

void Object::release(Mesh_Manager* mesh_manage)
{
	if(visible)
	{
		if(entity_mesh)
		{
			mesh_manage->release_mesh(entity_mesh);
			entity_mesh = NULL;
		}
	}
}

void Object::render(LPDIRECT3DDEVICE9 device)
{
	//This method is in charge of creating the matrix needed to draw the model
	//in the correct position and with to correct rotations and scale.
	//It also sets this matrix as the world transform and renders the mesh
	if(visible)
	{
		if(entity_mesh)
		{
			D3DXMATRIX world;
			D3DXMATRIX scale;
			D3DXMATRIX rotation_matrix;
			D3DXMATRIX translation;

			D3DXMatrixScaling(&scale, scale_factor, scale_factor, scale_factor);
			D3DXMatrixRotationQuaternion(&rotation_matrix, &rotation);
			D3DXMatrixTranslation(&translation, vector_position.x, vector_position.y, vector_position.z);

			world = scale * rotation_matrix * translation;

			device->SetTransform(D3DTS_WORLD, &world);

			entity_mesh->render(device);
		}
	}
}