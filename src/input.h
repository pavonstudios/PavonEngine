#ifndef INPUT_CONTROLLER
#define INPUT_CONTROLLER

#ifdef VULKAN
#include <GLFW/glfw3.h>
#endif

#include <iostream>
class Engine;

struct Key{
	bool bIsPressed = false;
	bool Released = false;
};
class Input {
public:

	Key D;
	Key A;
	Key E;
	Key Q;
	Key X;
	Key Z;
	Key W = {false, false};
	Key S;
	Key C;
	Key V;
	Key TAB;
	Key KEY_1;

	float pitch = 0;
	float yaw = 0;

	bool right_button_pressed = false;

	bool first_mouse = true;

	float lastX = 400, lastY = 300;
	float mousex, mousey;
	bool left_button_pressed = false;
	bool left_button_release = false;

	bool move_camera = false;
#ifdef VULKAN
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
#endif
#ifdef ES2
	void key_code_verifier_pressed(int key_code);
	void key_code_verifier_released(int key_code);
	void key_verifier_pressed(char character);

	void key_verifier_released(char character);
private:
	void key_set(const char key, bool isPressed);
#endif
public:
	void mouse_movement(Engine *engine, float x, float y);

	void update_input(Engine *engine);

#ifdef ANDROID
	void input_event(AInputEvent* event);
#endif


	

};
#endif