#ifndef GUI_H
#define GUI_H


struct Button : UIElement{	
	Button(EMesh* mesh){
		this->mesh = mesh;
	}
	bool pressed = false;
	bool released = false;
};

struct UIElement{
	EMesh* mesh;
	vec2 position;
	vec2 size;	
};

class GUI{
	public:
		GUI(Engine* engine);
		~GUI();
		Engine* engine;
		EMesh* mesh;
		void update();
		void calculate_mouse_position();
		std::vector<UIElement*> elements;
};

#endif