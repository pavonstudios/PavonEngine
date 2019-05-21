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
	vec2 saved_position;	

};

struct Button : UIElement{	
	Button(EMesh* mesh){
		this->mesh = mesh;
	}
	bool pressed = false;
	bool released = false;
	bool move = false;
};



class GUI{
	public:
		GUI(Engine* engine);
		~GUI();
		Engine* engine;
		void update();
		void calculate_mouse_position();
		std::vector<UIElement*> elements;
		void update_elements_mvp();
		bool is_button_pressed(std::string name);
		void update_elemete_position(UIElement* element);
		void update_engine_meshes();
		std::vector<EMesh*> mesh_elements;
};

#endif