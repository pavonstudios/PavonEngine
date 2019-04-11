#ifndef THIRD_PERSON
#define THIRD_PERSON

#include "../engine.h"
class ThirdPerson{
	public:
		Engine* engine;
		EMesh* mesh;
		void move_forward();
		Input input;
		void update();

};

#endif