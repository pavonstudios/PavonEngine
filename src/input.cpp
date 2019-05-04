#include "engine.h"
#include "input.h"
#include "objects.h"
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
					engine->input.TAB.Released = false;
				}
				if(action == GLFW_RELEASE){
					engine->input.TAB.bIsPressed = false;
					engine->input.TAB.Released = true;
					/* if(!engine->edit_mode)
						engine->edit_mode = true;
					else
					{
						engine->edit_mode = false;
					} */
					
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
	
		engine->input.mouse_movement(engine,xpos,ypos);
	 
	
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
void Input::mouse_movement(Engine* engine, float xpos, float ypos){
	//std::cout << "move to: "<< xpos << "," << ypos << std::endl;
	engine->input.mousex = xpos;
	engine->input.mousey = ypos;

	if(engine->input.move_camera){
		if(engine->input.first_mouse){
			engine->input.lastX = xpos;
			engine->input.lastY = ypos;
			engine->input.first_mouse = false;
		}
		std::cout << xpos << std::endl;

		float xoffset = xpos - engine->input.lastX ;

		std::cout << xoffset << std::endl;

		float yoffset = engine->input.lastY - ypos; // reversed since y-coordinates range from bottom to top
	
		if(engine->input.lastX != xpos && engine->input.lastY != ypos ){

				engine->input.yaw   = xoffset;
				engine->input.pitch = yoffset;  

				if(engine->input.pitch > 89.0f)
					engine->input.pitch =  89.0f;
				if(engine->input.pitch < -89.0f)
					engine->input.pitch = -89.0f;	
				
				
		}else{
				engine->input.yaw   = 0;
				engine->input.pitch = 0;  
		}

		engine->input.lastX = xpos;
		engine->input.lastY = ypos;

		

	
		

	}//end right click pressed
}	
void Input::key_verifier_pressed(char character){	
	key_set(character,true);
}

void Input::key_verifier_released(char character){	
	key_set(character,false);
}

void Input::key_set(const char key, bool isPressed){
	key_pressed *actual_key;
	if(key == 'w'){
		actual_key = &this->W;
	}
	if(key == 's'){
		actual_key = &this->S;
	}
	if(key == 'd'){
		actual_key = &this->D;
	}
	if(key == 'a'){
		actual_key = &this->A;
	}
	if(key == 'q'){
		actual_key = &this->Q;
	}
	if(key == 'e'){
		actual_key = &this->E;
	}
	if(key == 'p'){
		actual_key = &this->TAB;
		std::cout << "ptab\n";
	}
	if(key == 'z'){
		actual_key = &this->Z;
	}
	if(key == 'x'){
		actual_key = &this->X;
	}

	if(actual_key){
		if(isPressed){
		actual_key->bIsPressed = true;
		}else{
			actual_key->bIsPressed = false;
		}
	}
}

void Input::update_input(Engine* engine){
	if(TAB.Released){
		if(!engine->edit_mode)
			engine->edit_mode = true;
		else
		{
			engine->edit_mode = false;
		}
		TAB.Released = false;
#ifdef DEVELOPMENT
		engine->print_debug("tab pressed or released",0,0);
#endif
	}

	#ifdef ES2
	if(TAB.bIsPressed){
		if(engine->edit_mode){
			
		}
			//engine->edit_mode = false;
		else{
				engine->edit_mode = true;
				std::cout << "edidt mode \n";
		}
	}
	#endif

	if(engine->edit_mode){
		if(S.bIsPressed){
			engine->main_camera.MoveBackward();				
		}

		if(W.bIsPressed){
			engine->main_camera.MoveForward();
		}

		if(A.bIsPressed){		
			engine->main_camera.MoveLeft();
		}

		if(D.bIsPressed){		
			engine->main_camera.MoveRight();	
		}
#ifndef  ANDROID
		if(Z.bIsPressed){
						mat4 model_space = mat4(1.0);       
			mat4 rot = rotate(model_space,radians(0.009f),vec3(0,0,1));
			EMesh* smesh = engine->skeletal_meshes[0];

				Node* joint = smesh->skins[0]->joints[2];
			joint->matrix = translate(joint->matrix,vec3(0,0,0.0000)) * rot;
					Skeletal::update_joint_matrix(joint);
			
			smesh->node_uniform.joint_matrix[2] = inverse(smesh->model_matrix) * smesh->skins[0]->joints[2]->global_matrix;
				Node* joint2 = smesh->skins[0]->joints[3];
			
					Skeletal::update_joint_matrix(joint2);
			
			smesh->node_uniform.joint_matrix[3] = inverse(smesh->model_matrix) * smesh->skins[0]->joints[3]->global_matrix;
		}
		if(X.bIsPressed){	
						mat4 model_space = mat4(1.0);       
			mat4 rot = rotate(model_space,radians(-0.009f),vec3(0,0,1));
			EMesh* smesh = engine->skeletal_meshes[0];

				Node* joint = smesh->skins[0]->joints[2];
			joint->matrix = translate(joint->matrix,vec3(0,0,0.0000)) * rot;
					Skeletal::update_joint_matrix(joint);
			
			smesh->node_uniform.joint_matrix[2] = inverse(smesh->model_matrix) * smesh->skins[0]->joints[2]->global_matrix;
				Node* joint2 = smesh->skins[0]->joints[3];
			
					Skeletal::update_joint_matrix(joint2);
			
			smesh->node_uniform.joint_matrix[3] = inverse(smesh->model_matrix) * smesh->skins[0]->joints[3]->global_matrix;
		}
#endif
		if(Q.bIsPressed){
			engine->main_camera.MoveDown();
		}
		if(E.bIsPressed){
			engine->main_camera.MoveUp();
		}

		if(move_camera){
			engine->main_camera.mouse_control_update(yaw, pitch);

		}		

	}
}
#ifdef ANDROID
void Input::input_event(AInputEvent* event){
    float x =  AMotionEvent_getX(event,0);
    float y = AMotionEvent_getY(event,0);
    this->mousex = x;
    this->mousey = y;
    LOGW("touch in x = %f y = %f",x,y);
}
#endif