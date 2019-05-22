#include "ThirdPerson.hpp"
#include "weapon.hpp"
#ifndef ANDROID
    #include <glm/glm.hpp>
    #include <glm/gtc/matrix_transform.hpp>
#else
    #include "../glm/glm.hpp"
    #include "../glm/gtc/matrix_transform.hpp"
#endif


ThirdPerson::ThirdPerson(){

}

void ThirdPerson::update_camera_postion(){
	
	glm::mat4 mat = glm::translate(glm::mat4(1.0),glm::vec3(0,0,0));
	
	glm::mat4 rotated = glm::rotate(mat,glm::radians(90.f),glm::vec3(0,1,0));
	glm::mat4 rotated2 = glm::rotate(rotated,glm::radians(90.f),glm::vec3(0,1,0));
	glm::mat4 rotated3 = glm::rotate(rotated2,glm::radians(90.f),glm::vec3(-1,0,0));

	glm::mat4 translated =glm::translate(glm::mat4(1.0),camera_position);


	this->engine->main_camera.View = translated * rotated3 * glm::inverse(this->mesh->model_matrix);
	//engine->main_camera.cameraPos = camera_position + mesh->location_vector;
	
}

void ThirdPerson::update_weapon_position(){
	EMesh* weapon = engine->mesh_manager.mesh_by_name("glock");
	//mat4 hand_mat = 
	Node* hand = NodeManager::node_by_name(mesh,"Bone.012"); //TODO: node by name with skeletal parameter not updated
	mat4 hand_local = NodeManager::get_global_matrix(hand);
	

	mat4 front_rotated = rotate(mat4(1.0),radians(-90.f),vec3(0,0,1));
	mat4 front_rotated2 = rotate(mat4(1.0),radians(-90.f),vec3(0,0,1));
	mat4 front_rotated3 = rotate(mat4(1.0),radians(60.f),vec3(0,1,0));
	mat4 front_rotated4 = rotate(mat4(1.0),radians(-45.f),vec3(0,0,1));
	mat4 front_rotated5 = rotate(mat4(1.0),radians(16.f),vec3(0,1,0));

	mat4 rotation = front_rotated * front_rotated2 * front_rotated3 * front_rotated4 * front_rotated5;
	weapon->model_matrix =  mesh->model_matrix * hand_local * rotation ;
}

void ThirdPerson::update(){

		engine->play_animations = false;
		
	if(engine->input.left_button_pressed){
		weapons->shoot(weapons->main_weapon);
		std::cout << "shooting\n";
	}
	if(engine->input.R.Released){		
		engine->animation_manager.add_to_queue(mesh->skeletal,"reload",false);
		weapons->reload(weapons->main_weapon);
		engine->input.R.Released = false;
		std::cout << "reloading\n";
	}
	std::string log_position = "position " + std::to_string(mesh->location_vector.x) + " " + std::to_string(mesh->location_vector.y) + " " + std::to_string(mesh->location_vector.z);
	//engine->print_debug(log_position,10,0);	
	velocity = 2;

	#if defined(LINUX) && defined(X11) && defined (ES2)
	engine->window_manager.move_cursor_to_center();
	#endif
	if(engine->input.W.bIsPressed){
		if(engine->input.SHIFT.bIsPressed){
			velocity = 7;
			engine->animation_manager.play_animation(mesh->skeletal,"run",true);
		}else{
			engine->animation_manager.play_animation(mesh->skeletal,"walk",true);

		}
		engine->translate_mesh(mesh,FORWARD,velocity);
	}

	if(engine->input.D.bIsPressed){
		engine->translate_mesh(mesh,RIGTH,velocity);
	}

	if(engine->input.A.bIsPressed){
		engine->translate_mesh(mesh,LEFT,velocity);
	}

	if(engine->input.S.bIsPressed){
		engine->translate_mesh(mesh,BACKWARD,velocity);
	}	

	if(engine->input.Z.bIsPressed){		
	
	}
	if(engine->input.X.bIsPressed){
		
		//this->mesh->model_matrix = glm::rotate(mesh->model_matrix,glm::radians(-15.0f * engine->deltaTime),glm::vec3(0,0,1));
		 		
	}
	if(engine->input.SPACE.Released){				
		engine->animation_manager.add_to_queue(mesh->skeletal,"jump",false);
		engine->input.SPACE.Released = false;
	}

	if(engine->input.right_button_pressed){
		engine->animation_manager.play_animation(mesh->skeletal,"aim",false);
	}
	

	#ifdef DEVELOPMENT
		if(engine->input.Q.bIsPressed){		
			engine->translate_mesh(mesh,UP,velocity);		
		}
		if(engine->input.E.bIsPressed){
			engine->translate_mesh(mesh,DOWN,velocity);
		}
	#endif
	

	update_camera_postion();
	engine->input.move_camera = true;
	mouse_control(engine->input.yaw, engine->input.pitch);
	
	update_weapon_position();

}

void ThirdPerson::mouse_control(float yaw, float pitch){
	
	float rotation = (yaw * -5.f) * engine->deltaTime;
	if(yaw != 0){
		this->mesh->model_matrix = glm::rotate(mesh->model_matrix,glm::radians(rotation),glm::vec3(0,0,1));
	}
}

