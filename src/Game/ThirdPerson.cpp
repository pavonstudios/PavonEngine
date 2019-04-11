#include "ThirdPerson.hpp"

void ThirdPerson::move_forward(){
	this->engine->main_camera.MoveForward();
	this->mesh->model_matrix = glm::translate(mesh->model_matrix,glm::vec3(0,0.001,0));
	
}
void ThirdPerson::update(){
	engine->print_debug("moving around",10,0);
	if(input.W.bIsPressed){
		move_forward();
		
	}
}