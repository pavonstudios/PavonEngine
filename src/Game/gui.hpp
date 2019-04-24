#ifndef GUI_H
#define GUI_H

#include "../engine.h"

class GUI{
	public:
		GUI();
		Engine* engine;
		EMesh* mesh;
		void update();
};

#endif