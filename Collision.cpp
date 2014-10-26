#include "Collision.h"
#include "Collision_Sphere.h"

/* Not actually using this might convert to using function when there are more collision types
D3DXVECTOR3 calculate_true_position(D3DXVECTOR3 position, D3DXVECTOR3 offset)
{
	return position + offset;
}
*/

Collision::Collision() 
{
	this->position = D3DXVECTOR3(0, 0, 0);
	this->type = DOT;
}

bool Collision::check_collision(Collision* target1, Collision* target2)
{
	//determine type of collision check to perform sphere/sphere, box/sphere, box/box etc
	if(target1->get_type() == SPHERE && target2->get_type() == SPHERE)
	{
		Collision_Sphere* sphere1 = (Collision_Sphere*)target1;
		Collision_Sphere* sphere2 = (Collision_Sphere*)target2;

		if(target1->get_postion() == 0 || target2->get_postion() == 0)
		{
			return FALSE;
		}

		//calculate true position of targets
		D3DXVECTOR3 sphere1_location = sphere1->get_offset() + sphere1->get_postion();
		D3DXVECTOR3 sphere2_location = sphere2->get_offset() + sphere2->get_postion();

		//calculate distance between targets
		D3DXVECTOR3 difference = sphere1_location - sphere2_location;
		float distance = D3DXVec3Length(&difference);

		//check if distance between targets is smaller that either radius
		if(distance < sphere1->get_radius() || distance < sphere2->get_radius())
		{
			return TRUE;
		}
	}
	return FALSE;
}