#ifndef THIRD_PERSON
#define THIRD_PERSON

#include "../engine.h"

class ThirdPerson{
	public:
		ThirdPerson();
		Engine* engine;
		EMesh* mesh;
		void update_camera_postion();
		void monse_control(float yaw, float pitch);
		void update();
		glm::vec3 position = glm::vec3(0,0,0);
};

#endif