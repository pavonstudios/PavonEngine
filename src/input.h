#ifndef INPUT_CONTROLLER
#define INPUT_CONTROLLER

#include <GLFW/glfw3.h>
#include <iostream>

struct key_pressed{
	bool bIsPressed = false;
};
class Input{
public:
	
	key_pressed D;
	key_pressed A;
	key_pressed E;
	key_pressed Q;
	key_pressed X;
	key_pressed Z;
	key_pressed W;
	key_pressed S;
	key_pressed TAB;

	float pitch = 0;
	float yaw = 0;

	bool right_button_pressed = false;

	bool first_mouse = true;

	float lastX = 400, lastY = 300;

	bool move_camera = false;

	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

};
#endif