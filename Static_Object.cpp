#include "Static_Object.h"

Static_Object::Static_Object(Mesh* model, variable_map* constructor_settings, variable_map* variable_settings)
		 : Object(model, constructor_settings, variable_settings)
{
	reload_variables();
}

void Static_Object::update(float timestep)
{
	if(rotate_speed != 0)
	{
		D3DXQUATERNION rotation_test;
		D3DXQuaternionRotationYawPitchRoll(&rotation_test, timestep * rotate_speed, 0.0f, 0.0f);
		D3DXQuaternionNormalize(&rotation_test, &rotation_test);
		rotation *= rotation_test;
	}
}

void Static_Object::reload_variables()
{
	rotate_speed = boost::any_cast<float>(variable_settings->at("rotate_speed"));
}