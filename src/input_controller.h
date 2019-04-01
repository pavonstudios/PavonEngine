#ifndef INPUT_CONTROLLER
#define INPUT_CONTROLLER
struct key_pressed{
	bool bIsPressed;
};
class Input{
public:
	bool bIsKeyS_pressed = false;
	bool bIsKeyW_pressed = false;

	key_pressed D;
	key_pressed A;

	float pitch = 0;
	float yaw = 0;

	bool right_button_pressed = false;

	bool first_mouse = true;

	float lastX = 400, lastY = 300;

	key_pressed X;
	key_pressed Z;

};
#endif