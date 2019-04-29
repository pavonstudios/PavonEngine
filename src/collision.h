#ifndef COLLISION
#define COLLISION
#include "engine.h"

class Collision{
public:
	static bool detect_point(const AABB& tBox, glm::vec3 vecPoint){
			//Check if the point is less than max and greater than min
		bool y = false;
		bool z = false;
		bool x = false;

		if(vecPoint.x >= tBox.m_vecMin.x && vecPoint.x <= tBox.m_vecMax.x){
			//std::cout << "x aligment\n";
			x = true;
		} 
		if(vecPoint.y >= tBox.m_vecMin.y && vecPoint.y <= tBox.m_vecMax.y){
			//std::cout << "y aligment\n";
			y = true;
		}
		if(vecPoint.z >= tBox.m_vecMin.z && vecPoint.z <= tBox.m_vecMax.z){
			//std::cout << "z aligment\n";
			z = true;
		}
		if(x & y & z){
			return true;
		}
		

		//If not, then return false
		return false;
	};

	static bool detect_point(const AABB& tBox, EMesh* mesh){
		//Check if the point is less than max and greater than min
		bool y = false;
		bool z = false;
		bool x = false;

		mesh->collider.collision = false;
		mesh->collider.positive_x = false;
		mesh->collider.negative_x = false;

		glm::vec3 vecPoint = mesh->location_vector;

		if(vecPoint.x + mesh->box.m_vecMax.x >= tBox.m_vecMin.x &&
			vecPoint.x - mesh->box.m_vecMin.x <= tBox.m_vecMax.x){
				x = true;
				mesh->collider.positive_x = true;
			}

		if(vecPoint.x - mesh->box.m_vecMax.x >= tBox.m_vecMin.x &&
		vecPoint.x + mesh->box.m_vecMin.x <= tBox.m_vecMax.x){
			x = true;
			mesh->collider.negative_x = true;
		}

		if(vecPoint.y + mesh->box.m_vecMax.y >= tBox.m_vecMin.y &&
		vecPoint.y - mesh->box.m_vecMin.y <= tBox.m_vecMax.y){
			y = true;
		}

		if(vecPoint.y - mesh->box.m_vecMax.y >= tBox.m_vecMin.y &&
		vecPoint.y + mesh->box.m_vecMin.y <= tBox.m_vecMax.y){
			y = true;
		}

		if(vecPoint.z + mesh->box.m_vecMax.z >= tBox.m_vecMin.z &&
			vecPoint.z - mesh->box.m_vecMin.z <= tBox.m_vecMax.z){
				z = true;
			}


		{//center point
			if(vecPoint.x >= tBox.m_vecMin.x && vecPoint.x <= tBox.m_vecMax.x){
				//std::cout << "x aligment\n";
				x = true;
			} 
			if(vecPoint.y >= tBox.m_vecMin.y && vecPoint.y <= tBox.m_vecMax.y){
				//std::cout << "y aligment\n";
				y = true;
			}
			if(vecPoint.z >= tBox.m_vecMin.z && vecPoint.z <= tBox.m_vecMax.z){
				//std::cout << "z aligment\n";
				z = true;
			}
		}

		if(x & y & z){
			mesh->collider.collision = true;
			return true;
		}
		
		mesh->collider.collision = false;
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

	 static void update_collision(EMesh* mesh, EMesh* mesh2){
		std::cout << "start collision engine\n";
	
		while(1){
			if(Collision::detect_point(mesh2->box,mesh)){
				std::cout << "collision\n";
			}
		}
		std::cout << "finish thread\n";
	 }
};
#endif