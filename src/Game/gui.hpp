#ifndef GUI_H
#define GUI_H

#define POSITION_RELATIVE_LEFT_BOTTON 0
#define POSITION_RELATIVE_LEFT_TOP 1

struct UIElement{
public:
	std::string name;
	EMesh* mesh;
	vec2 position;
	vec2 relative_position;
	vec2 size;
	int relative_to = -1;	

};

struct Button : UIElement{	
	Button(EMesh* mesh){
		this->mesh = mesh;
	}
	bool pressed = false;
	bool released = false;
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
		void update_elements_mvp();
		bool is_button_pressed(std::string name);
};

#endif