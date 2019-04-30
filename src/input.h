#ifndef INPUT_CONTROLLER
#define INPUT_CONTROLLER

#ifdef VULKAN
#include <GLFW/glfw3.h>
#endif

#include <iostream>
class Engine;

struct key_pressed{
	bool bIsPressed = false;
	bool Released = false;
};
class Input{
public:
	
	key_pressed D;
	key_pressed A;
	key_pressed E;
	key_pressed Q;
	key_pressed X;
	key_pressed Z;
	key_pressed W = {false,false};
	key_pressed S;
	key_pressed TAB;

	float pitch = 0;
	float yaw = 0;

	bool right_button_pressed = false;

	bool first_mouse = true;

	float lastX = 400, lastY = 300;
	float mousex, mousey;
	bool left_button_pressed = false;

	bool move_camera = false;
	#ifdef VULKAN
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	#endif

	void key_verifier_pressed(char character);
	void key_verifier_released(char character);
	void mouse_movement(Engine* engine, float x, float y);
	void update_input(Engine* engine);
private:
	void key_set(const char key, bool isPressed);
	

};
#endif