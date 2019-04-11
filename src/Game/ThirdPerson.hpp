#ifndef THIRD_PERSON
#define THIRD_PERSON

#include "../engine.h"
class ThirdPerson{
	public:
		Engine* engine;
		EMesh* mesh;
		void move_forward();
		void update();

};

#endif