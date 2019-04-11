#include "input_controller.h"
#include "3D_objects.h"
#include "camera.h"

#include "engine.h"
void Input::scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
			#ifdef VULKAN
	  			auto engine = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(window));
			#endif
			if(engine)
				engine->main_camera.velocity += yoffset;

}