#include "camera.h"
void Camera::MoveForward(){
	 cameraPos += cameraSpeed * cameraFront;
	View = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Camera::MoveBackward(){
	cameraPos -= cameraSpeed * cameraFront;
	View = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Camera::MoveLeft(){
	cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		View = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	
}

void Camera::MoveRight(){
	
	cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	View = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);



}
void Camera::mouse_control_update(float yaw, float pitch){
	glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.z = sin(glm::radians(pitch));
    front.y = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);

	View = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}
