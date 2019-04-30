#include "vehicle.hpp"

void Vehicle::update(){
	if(engine->input.W.bIsPressed == true){
		throttle();
	}else{
		gas = false;
	}
	
	if(engine->input.S.bIsPressed){
		velocity = velocity - (friction+30) * engine->deltaTime;
	}	

	if(engine->input.A.bIsPressed){
		
		this->mesh->model_matrix = glm::rotate(mesh->model_matrix,glm::radians(15.0f * engine->deltaTime),glm::vec3(0,0,1));
		
	}
	if(engine->input.D.bIsPressed){
		
		this->mesh->model_matrix = glm::rotate(mesh->model_matrix,glm::radians(-15.0f * engine->deltaTime),glm::vec3(0,0,1));
		
	}

	update_velocity();
	update_camera_postion();
}
void Vehicle::update_velocity(){
		
	if(velocity > 2){
		engine->translate_mesh(mesh,FORWARD,velocity);
		if(!gas) if(velocity > 5) reduce_velocity();
	}

		
		
	
	if(velocity < 0){
		float v = velocity * -1;
		engine->translate_mesh(mesh,BACKWARD,v);
	}
}

void Vehicle::throttle(){
	velocity = velocity + (torque * engine->deltaTime);
	gas = true;
}
void Vehicle::reduce_velocity(){
	if(!velocity < 1){
		if(velocity > 2){
			
			velocity = velocity - (friction * engine->deltaTime);
					
			
		}
	}
	
	
}