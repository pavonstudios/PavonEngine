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
#ifdef VULKAN
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.z = sin(glm::radians(pitch));
    front.y = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    
#else
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
#endif
	cameraFront = glm::normalize(front);
	//cameraPos = glm::normalize(glm::cross(cameraFront, cameraUp));

	View = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Camera::MoveDown(){
	cameraPos -= cameraSpeed * cameraUp;
	View = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

}

void Camera::MoveUp(){
	cameraPos += cameraSpeed * cameraUp;
	View = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}
