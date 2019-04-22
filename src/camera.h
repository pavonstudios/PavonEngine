#ifndef CAMERA_H
#define CAMERA_H

#include "game_object.h"
#ifndef ANDROID
	#include <glm/glm.hpp>
	#include <glm/gtc/matrix_transform.hpp>
#else
	#include "glm/glm.hpp"
	#include "glm/gtc/matrix_transform.hpp"
#endif

class Camera : public GameObject{
public:
	float screen_height = 600;
	float screen_width = 800;
	glm::vec3 cameraPos   = glm::vec3(0.0f, -25.0f,  0.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraUp    = glm::vec3(0.0f, 0.0f,  1.0f);

	glm::mat4 Projection = glm::perspective(glm::radians(45.f), screen_width/screen_height, 0.01f, 5000.f);

	glm::mat4 View = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	virtual void SetLocation(float x, float y, float z){
		Location.x = x;
		Location.y = y;
		Location.z = z;
		View = glm::lookAt(
				glm::vec3(Location.x,Location.y,Location.z),// Camera is at (4,3,3), in World Space
				glm::vec3(0,0,0), // and looks at the origin
				glm::vec3(0,0,1)  // Head is up (set to 0,-1,0 to look upside-down)
			);
	
	}
	
	void update_projection_matrix(){
	Projection = glm::perspective(glm::radians(45.f), screen_width/screen_height, 0.01f, 5000.f);
	}
	void update(){
		View = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	};		
	
	float cameraSpeed = 0.05f; 
	float velocity = 5.f;
	void MoveForward();
	void MoveBackward();
	void MoveLeft();
	void MoveRight();
	void mouse_control_update(float , float);
	void MoveUp();
	void MoveDown();
};

#endif
