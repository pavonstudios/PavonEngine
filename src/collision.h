#ifndef COLLISION
#define COLLISION
#include "engine.h"

class Collision{
public:
	static bool detect_point(const AABB& tBox, glm::vec3 vecPoint){
			//Check if the point is less than max and greater than min
		if(vecPoint.x > tBox.m_vecMin.x && vecPoint.x < tBox.m_vecMax.x &&
		vecPoint.y > tBox.m_vecMin.y && vecPoint.y < tBox.m_vecMax.y &&
		vecPoint.z > tBox.m_vecMin.z && vecPoint.z < tBox.m_vecMax.z)
		{
			return true;
		}

	//If not, then return false
	return false;
	};
};
#endif