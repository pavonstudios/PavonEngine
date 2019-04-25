#include "ThirdPerson.hpp"
#ifndef ANDROID
    #include <glm/glm.hpp>
    #include <glm/gtc/matrix_transform.hpp>
#else
    #include "../glm/glm.hpp"
    #include "../glm/gtc/matrix_transform.hpp"
#endif
ThirdPerson::ThirdPerson(){

}

void ThirdPerson::move_forward(){
	//this->engine->main_camera.MoveForward();
	glm::mat4 mat = glm::translate(glm::mat4(1.0),glm::vec3(0,0,0));
	
	glm::mat4 rotated = glm::rotate(mat,glm::radians(90.f),glm::vec3(0,1,0));
	glm::mat4 rotated2 = glm::rotate(rotated,glm::radians(90.f),glm::vec3(0,1,0));
	glm::mat4 rotated3 = glm::rotate(rotated2,glm::radians(90.f),glm::vec3(-1,0,0));

	glm::mat4 translated =glm::translate(glm::mat4(1.0),glm::vec3(0,-1.5,-2.5));


	this->engine->main_camera.View = translated * rotated3 * glm::inverse(this->mesh->model_matrix);
	
}
void ThirdPerson::update(){
	move_forward();
#ifndef ANDROID
	if(engine->input.W.bIsPressed == true){
		
		this->mesh->model_matrix = glm::translate(mesh->model_matrix,glm::vec3(0,-2 * engine->deltaTime,0));
		engine->print_debug("moving around",10,0);
	}
	if(engine->input.Z.bIsPressed){
		
		this->mesh->model_matrix = glm::rotate(mesh->model_matrix,glm::radians(0.008f),glm::vec3(0,0,1));
		
	}
	monse_control(engine->input.yaw, engine->input.pitch);
#endif
}
void ThirdPerson::monse_control(float yaw, float pitch){
	this->mesh->model_matrix = glm::rotate(mesh->model_matrix,glm::radians(-yaw * (2.f * engine->deltaTime)),glm::vec3(0,0,1));
}