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
		vec3 location;
	private:
		mat4 camera_rotation_offset = mat4(1.0);
		mat4 weapon_rotation_offset = mat4(1.0);
		void create_rotation_offset_matrixs();

};

#endif