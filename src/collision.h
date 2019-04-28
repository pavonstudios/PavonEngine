#ifndef COLLISION
#define COLLISION
#include "engine.h"

class Collision{
public:
	static bool detect_point(const AABB& tBox, glm::vec3 vecPoint){
			//Check if the point is less than max and greater than min
		if(vecPoint.x >= tBox.m_vecMin.x && vecPoint.x <= tBox.m_vecMax.x &&
		vecPoint.y >= tBox.m_vecMin.y && vecPoint.y <= tBox.m_vecMax.y &&
		vecPoint.z >= tBox.m_vecMin.z && vecPoint.z <= tBox.m_vecMax.z)
		{
			return true;
		}

	//If not, then return false
	return false;
	};
	static bool detect_aabbs(const AABB& tBox1, const AABB& tBox2){
		//Check if Box1's max is greater than Box2's min and Box1's min is less than Box2's max
    return(
	tBox1.m_vecMax.x > tBox2.m_vecMin.x &&
    tBox1.m_vecMin.x < tBox2.m_vecMax.x &&
    tBox1.m_vecMax.y > tBox2.m_vecMin.y &&
    tBox1.m_vecMin.y < tBox2.m_vecMax.y &&
    tBox1.m_vecMax.z > tBox2.m_vecMin.z &&
    tBox1.m_vecMin.z < tBox2.m_vecMax.z);

	//If not, it will return false
	};
};
#endif