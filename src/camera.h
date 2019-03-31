#include "game_object.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Camera : public GameObject{
public:
	glm::mat4 Projection = glm::perspective(glm::radians(45.f), 800.f/600.f, 0.1f, 100.f);
	glm::mat4 View = glm::lookAt(
		//glm::vec3(Location.x,Location.y,Location.z), 
		glm::vec3(0,0,0),// Camera is at (4,3,3), in World Space
		glm::vec3(0,0,0), // and looks at the origin
		glm::vec3(0,0,1)  // Head is up (set to 0,-1,0 to look upside-down)
    );
	virtual void SetLocation(float x, float y, float z){
		Location.x = x;
		Location.y = y;
		Location.z = z;
		View = glm::lookAt(
			glm::vec3(Location.z,Location.y,Location.z),// Camera is at (4,3,3), in World Space
			glm::vec3(0,0,0), // and looks at the origin
			glm::vec3(0,0,1)  // Head is up (set to 0,-1,0 to look upside-down)
    );
	}
	

};
