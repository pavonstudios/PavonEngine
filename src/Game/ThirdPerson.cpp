#include "ThirdPerson.hpp"

void ThirdPerson::move_forward(){
	this->engine->main_camera.MoveForward();
	this->mesh->model_matrix = glm::translate(mesh->model_matrix,glm::vec3(0,0.001,0));
	
}
void ThirdPerson::update(){
	
	if(engine->input.W.bIsPressed){
		move_forward();
		engine->print_debug("moving around",10,0);
	}
}