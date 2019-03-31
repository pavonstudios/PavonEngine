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

};
#endif