#ifndef THIRD_PERSON
#define THIRD_PERSON

#include "../engine.h"
class ThirdPerson{
	public:
		Engine* engine;
		EMesh* mesh;
		void move_forward();
		void monse_control(float yaw, float pitch);
		void update();

};

#endif