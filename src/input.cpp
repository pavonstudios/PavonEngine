#include "engine.h"
#include "input.h"
#include "3D_objects.h"
#include "camera.h"

#ifdef VULKAN
void Input::scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
			#ifdef VULKAN
	  			auto engine = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(window));
			
			if(engine){
				engine->main_camera.velocity += yoffset;
			}
			#endif
}

void Input::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	#ifdef VULKAN
		auto engine = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(window));
	
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
           #endif
           
}
void Input::mouse_callback(GLFWwindow* window, double xpos, double ypos){
	#ifdef VULKAN
	  auto engine = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(window));
	
		
	
	if(engine->input.move_camera){
		if(engine->input.first_mouse){
			engine->input.lastX = xpos;
			engine->input.lastY = ypos;
			engine->input.first_mouse = false;
		}

		float xoffset = xpos - engine->input.lastX ;
		float yoffset = engine->input.lastY - ypos; // reversed since y-coordinates range from bottom to top
		engine->input.lastX = xpos;
		engine->input.lastY = ypos;

		float sensitivity = 0.05f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		engine->input.yaw   += xoffset;
		engine->input.pitch += yoffset;  

		if(engine->input.pitch > 89.0f)
			engine->input.pitch =  89.0f;
		if(engine->input.pitch < -89.0f)
			engine->input.pitch = -89.0f;		

	}//end right click pressed
	
	#endif

}

void Input::mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
	#ifdef VULKAN
		auto engine = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(window));
	#endif

		if (button == GLFW_MOUSE_BUTTON_RIGHT ){
			if(action == GLFW_PRESS){
				engine->input.right_button_pressed = true;
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);  
				engine->input.move_camera = false;
			}
			if(action == GLFW_RELEASE){
				engine->input.right_button_pressed = false;
			}

		}
		if (button == GLFW_MOUSE_BUTTON_LEFT ){
			if(action == GLFW_PRESS){
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
				engine->input.move_camera = true; 
			}
			if(action == GLFW_RELEASE){
				//app->engine->input.right_button_pressed = false;
			}

		}

}
#endif//end if define vulkan

void Input::key_verifier_pressed(char character){
	if(character == 'w'){
		this->W.bIsPressed = true;
	}
}

void Input::key_verifier_released(char character){
	if(character == 'w'){
		this->W.bIsPressed = false;
	}
}