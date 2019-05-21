#ifndef THIRD_PERSON
#define THIRD_PERSON

#include "../engine.h"

class WeaponManager;

class ThirdPerson{
	public:
		ThirdPerson();
		Engine* engine;
		EMesh* mesh;
		SkeletalMesh* skeletal = nullptr;
		WeaponManager* weapons;
		void update_camera_postion();
		void mouse_control(float yaw, float pitch);
		void update_weapon_position();
		virtual void update();
		float velocity = 2;
		vec3 camera_position = vec3(0,-1.5,-1.5);		
};

#endif