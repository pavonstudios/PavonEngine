#include "ThirdPerson.hpp"

void ThirdPerson::move_forward(){
	this->engine->main_camera.MoveForward();
	this->mesh->model_matrix = glm::translate(mesh->model_matrix,glm::vec3(0,0.001,0));
	
}
void ThirdPerson::update(){
	if(input.W.bIsPressed){
		move_forward();
		std::cout << "move character" << std::endl;
	}
}