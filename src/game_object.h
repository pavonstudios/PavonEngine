#ifndef _GameObject_H_
#define _GameObject_H_
struct Vector3{
	float x = 0;
	float y = 0;
	float z = 0;
};
struct Transform
{
	Vector3 Location; 
};

class GameObject{
public:
	Transform transform;
	Vector3 Location;
	virtual void SetLocation(float x, float y, float z){
		Location.x = x;
		Location.y = y;
		Location.z = z;
	}
};

class GameObjectsManager{
	public:
		//void load_and_instance_at_location(std::string path, glm::vec3 location);
};

#endif