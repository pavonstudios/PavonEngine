#include "camera.h"
void Camera::MoveForward(){
	 cameraPos += cameraSpeed * cameraFront;
	 update_look_at();
}

void Camera::MoveBackward(){
	cameraPos -= cameraSpeed * cameraFront;
	update_look_at();
}

void Camera::MoveLeft(){
	cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	update_look_at();
	
}

void Camera::MoveRight(){
	
	cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;


	update_look_at();

}
void Camera::mouse_control_update(float yaw, float pitch){
	glm::vec3 front;
#ifdef VULKAN
    front.x = cos(glm::radians(-yaw)) * cos(glm::radians(pitch));
    front.z = sin(glm::radians(pitch));
    front.y = sin(glm::radians(-yaw)) * cos(glm::radians(pitch));
    
#else
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
#endif
	cameraFront = glm::normalize(front);
	//cameraPos = glm::normalize(glm::cross(cameraFront, cameraUp));

	update_look_at();
}

void Camera::MoveDown(){
	cameraPos -= cameraSpeed * cameraUp;
	update_look_at();
}

void Camera::update_look_at()
{
#ifndef DX11
	View = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
#else
	View = glm::lookAtLH(cameraPos, cameraPos + cameraFront, cameraUp);
#endif // !DX11

	
}

void Camera::MoveUp(){
	cameraPos += cameraSpeed * cameraUp;
	update_look_at();
}
