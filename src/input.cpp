#include "input_controller.h"
#include "3D_objects.h"
#include "camera.h"

#include "engine.h"
void Input::scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
			#ifdef VULKAN
	  			auto engine = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(window));
			#endif
			if(engine){
				engine->main_camera.velocity += yoffset;
			}
}

void Input::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	#ifdef VULKAN
		auto engine = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(window));
	#endif
			if(key == GLFW_KEY_TAB){
				if(action == GLFW_PRESS){
					engine->input.TAB.bIsPressed = true;
				}
				if(action == GLFW_RELEASE){
					engine->input.TAB.bIsPressed = false;
					if(!engine->edit_mode)
						engine->edit_mode = true;
					else
					{
						engine->edit_mode = false;
					}
					
				}
			}
			if(key == GLFW_KEY_S){
				if(action == GLFW_PRESS){
					engine->input.S.bIsPressed = true;
				}
				if(action == GLFW_RELEASE){
					engine->input.S.bIsPressed = false;
				}
			}
			if(key == GLFW_KEY_W){
				if(action == GLFW_PRESS){
					engine->input.W.bIsPressed = true;
				}
				if(action == GLFW_RELEASE){
					engine->input.W.bIsPressed = false;
				}
			}
			if(key == GLFW_KEY_D){
				if(action == GLFW_PRESS){
					engine->input.D.bIsPressed = true;
				}
				if(action == GLFW_RELEASE){
					engine->input.D.bIsPressed = false;
				}
			}
			if(key == GLFW_KEY_A){
				if(action == GLFW_PRESS){
					engine->input.A.bIsPressed = true;
				}
				if(action == GLFW_RELEASE){
					engine->input.A.bIsPressed = false;
				}
			}
			if(key == GLFW_KEY_Z){
				if(action == GLFW_PRESS){
					engine->input.Z.bIsPressed = true;
				}
				if(action == GLFW_RELEASE){
					engine->input.Z.bIsPressed = false;
				}
			}
			
			if(key == GLFW_KEY_E){
				if(action == GLFW_PRESS){
					engine->input.E.bIsPressed = true;
				}
				if(action == GLFW_RELEASE){
					engine->input.E.bIsPressed = false;
				}
			}
			if(key == GLFW_KEY_Q){
				if(action == GLFW_PRESS){
					engine->input.Q.bIsPressed = true;
				}
				if(action == GLFW_RELEASE){
					engine->input.Q.bIsPressed = false;
				}
			}
			if(key == GLFW_KEY_X){
				if(action == GLFW_PRESS){
					engine->input.X.bIsPressed = true;
				}
				if(action == GLFW_RELEASE){
					engine->input.X.bIsPressed = false;
				}
			}
           
           
}
