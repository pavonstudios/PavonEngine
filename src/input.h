#ifndef INPUT_CONTROLLER
#define INPUT_CONTROLLER

#if defined (VULKAN) || defined (DX11)
	#include <GLFW/glfw3.h>
#endif
#if defined(X11)
	#include  <X11/Xlib.h>
#endif
#ifdef WINDOWS
#include <Windows.h>
#endif
#include <iostream>
class Engine;

#define KEY_A 0x41
#define KEY_S 0x53
#define KEY_W 0x57
#define KEY_D 0x44
#define KEY_E 0x45
#define KEY_Q 0x51
#define KEY_X 0x58
#define KEY_Z 0x5A
#define KEY_F 0x46

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
	Key SPACE;
	Key ESC;
	Key SHIFT;
	Key R;

	float pitch = 0;
	float yaw = 0;

	bool right_button_pressed = false;
	bool right_button_release = false;
	
	bool first_mouse = true;

	float lastX = 400, lastY = 300;
	float mouse_in_gamex = 400, mouse_in_gamey = 300;
	float mousex, mousey;
	bool left_button_pressed = false;
	bool left_button_release = false;

	bool move_camera = false;
#ifdef WINDOWS
	void handle_input_windows_messages(WPARAM wParam);
	void handle_key_released(WPARAM wParam);
	void handle_key_pressed(WPARAM wParam);
	void handle_mouse_movement(Engine*, LPARAM);
#endif // WINDOWS

#if defined (VULKAN) || defined (DX11)
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
#endif
#if defined(LINUX) && defined(X11)
	void key_code_verifier_pressed(int key_code);
	void key_code_verifier_released(int key_code);
	void key_verifier_pressed(char character);

	void key_verifier_released(char character);

	void check_input_event(Engine* engine, XEvent &event);
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