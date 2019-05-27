#ifndef CAMERA_H
#define CAMERA_H

#ifndef ANDROID
	#include <glm/glm.hpp>
	#include <glm/gtc/matrix_transform.hpp>
#else
	#include "glm/glm.hpp"
	#include "glm/gtc/matrix_transform.hpp"
#endif

class Camera {
public:
	float screen_height = 600;
	float screen_width = 800;
	glm::vec3 cameraPos   = glm::vec3(0.0f, -5.0f, 4.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraUp    = glm::vec3(0.0f, 0.0f,  1.0f);

	glm::mat4 Projection;

	glm::mat4 View;
	
	Camera();
	void update_projection_matrix();
	void update();
	
	float cameraSpeed = 0.05f; 
	float velocity = 5.f;
	void MoveForward();
	void MoveBackward();
	void MoveLeft();
	void MoveRight();
	void mouse_control_update(float , float);
	void MoveUp();
	void MoveDown();

	void update_look_at();
};

#endif
