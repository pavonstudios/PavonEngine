#ifndef INPUT_CONTROLLER
#define INPUT_CONTROLLER
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

	float pitch = 0;
	float yaw = 0;

	bool right_button_pressed = false;

	bool first_mouse = true;

	float lastX = 400, lastY = 300;

	

};
#endif