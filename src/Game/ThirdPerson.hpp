#ifndef THIRD_PERSON
#define THIRD_PERSON

#include "../engine.h"

class ThirdPerson{
	private:
		bool movement_x = true;
		bool movement_x_negative = true;
	public:
		ThirdPerson();
		Engine* engine;
		EMesh* mesh;
		void update_camera_postion();
		void monse_control(float yaw, float pitch);
		void update();
		glm::vec3 position = glm::vec3(0,0,0);
		float velocity = 2;
};

#endif