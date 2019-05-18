#ifndef THIRD_PERSON
#define THIRD_PERSON

#include "../engine.h"

class ThirdPerson{
	public:
		ThirdPerson();
		Engine* engine;
		EMesh* mesh;
		void update_camera_postion();
		void mouse_control(float yaw, float pitch);
		virtual void update();
		glm::vec3 position = glm::vec3(0,0,0);
		float velocity = 2;
		vec3 camera_position = vec3(0,-1.5,-1.5);
		void connect_to_game_server();
};

#endif