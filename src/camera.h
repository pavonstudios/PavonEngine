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
	glm::vec3 cameraPos   = glm::vec3(0.0f, -6.0f,  2.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraUp    = glm::vec3(0.0f, 0.0f,  1.0f);

	glm::mat4 Projection = glm::perspective(glm::radians(45.f), 800.f/600.f, 0.1f, 5000.f);

	glm::mat4 View = glm::lookAt(
		cameraPos,// Camera is at (4,3,3), in World Space
		glm::vec3(0,0,0), // and looks at the origin
		glm::vec3(0,0,1)  // Head is up (set to 0,-1,0 to look upside-down)
    );
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

	void update(){
			View = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		};
		void rotate(){
			 /*   float radius = 10.0f;
        float camX = sin(glfwGetTime()) * radius;
        float camZ = cos(glfwGetTime()) * radius;
        glm::mat4 view;
        view = glm::lookAt(glm::vec3(camX, camZ, 0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0));   */
		};
		
		float cameraSpeed = 0.005f; 
		void MoveForward();
		void MoveBackward();
		void MoveLeft();
		void MoveRight();
		void mouse_control_update(float , float);

	

};
