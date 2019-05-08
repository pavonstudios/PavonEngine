#include "engine.h"
#include "collision.h"
#include "Game/game.hpp"
using namespace engine;


void Collision::create_collision_helper_vertices(Engine* engine){
	
}
bool Collision::sphere_collision_of(EMesh* mesh2 , EMesh* mesh){

		bool y = false;
		bool z = false;
		bool x = false;

		AABB tBox = mesh2->box;
		mesh->collider.collision = false;
		mesh->collider.positive_x = false;
		mesh->collider.negative_x = false;
		mesh->collider.positive_y = false;
		mesh->collider.negative_y = false;

		glm::vec3 vecPoint = mesh->location_vector;
		glm::vec3 mesh2_vec = mesh2->location_vector;

		

	
		if(x & y & z){
			mesh->collider.collision = true;
			return true;
		}
		
		mesh->collider.collision = false;
		
		return false;
};

bool Collision::collision_of(EMesh* mesh2 , EMesh* mesh){

		bool y = false;
		bool z = false;
		bool x = false;

		AABB tBox = mesh2->box;
		mesh->collider.collision = false;
		mesh->collider.positive_x = false;
		mesh->collider.negative_x = false;
		mesh->collider.positive_y = false;
		mesh->collider.negative_y = false;

		glm::vec3 vecPoint = mesh->location_vector;
		glm::vec3 mesh2_vec = mesh2->location_vector;

		if(vecPoint.x + mesh->box.m_vecMax.x >= mesh2_vec.x - mesh2->box.m_vecMax.x){
			mesh->collider.positive_x = true;
			if(vecPoint.y - mesh->box.m_vecMax.y <= mesh2_vec.y + mesh2->box.m_vecMax.y) {
				mesh->collider.negative_y = true;
				if(vecPoint.x + mesh->box.m_vecMin.x <= mesh2_vec.x - mesh2->box.m_vecMin.x){
					mesh->collider.negative_x = true;
					if(vecPoint.y - mesh->box.m_vecMin.y >= mesh2_vec.y + mesh2->box.m_vecMin.y){
						mesh->collider.positive_y = true;
						x =true;
						y = true;
						z = true;//TODO:
					}

				}
			}
		}
		
	
		if(x & y & z){
			mesh->collider.collision = true;
			return true;
		}
		
		mesh->collider.collision = false;
		
		return false;
};


bool Collision::trigger_on(EMesh* mesh2 , EMesh* mesh){

		bool y = false;
		bool z = false;
		bool x = false;

		AABB tBox = mesh2->box;
		mesh->collider.collision = false;
		mesh->collider.positive_x = false;
		mesh->collider.negative_x = false;
		mesh->collider.positive_y = false;
		mesh->collider.negative_y = false;

		glm::vec3 vecPoint = mesh->location_vector;
		glm::vec3 mesh2_vec = mesh2->location_vector;

		if(vecPoint.x + mesh->box.m_vecMax.x >= mesh2_vec.x - mesh2->box.m_vecMax.x){
			
			if(vecPoint.y - mesh->box.m_vecMax.y <= mesh2_vec.y + mesh2->box.m_vecMax.y) {
				
				if(vecPoint.x + mesh->box.m_vecMin.x <= mesh2_vec.x - mesh2->box.m_vecMin.x){
					
					if(vecPoint.y - mesh->box.m_vecMin.y >= mesh2_vec.y + mesh2->box.m_vecMin.y){
						
						x =true;
						y = true;
						z = true;//TODO:
					}

				}
			}
		}
		
	
		if(x & y & z){
			mesh->collider.collision = true;
			return true;
		}
		
		mesh->collider.collision = false;
		
		return false;
};

void Collision::update_collision(EMesh* mesh, EMesh* mesh2){
		std::cout << "starting collision engine\n";
	
		while(1){
			if(mesh && mesh2){
				if(Collision::collision_of(mesh,mesh2)){
					std::cout << "collision\n";
				}
			}		
		}
		std::cout << "finish thread\n";
}

void Collision::update_collision(const std::vector<EMesh*>& meshes){
		std::cout << "starting collision engine\n";
	
		while(1){
			/* if(mesh && mesh2){
				if(Collision::collision_of(mesh,mesh2)){
					std::cout << "collision\n";
				}
			}		 */
		}
		std::cout << "finish thread\n";
}

void Collision::update_collision_engine(const Engine* engine){

		std::cout << "starting collision engine\n";

		EMesh* player = engine->game->player->mesh;

		EMesh* vehicle = engine->meshes[3];

		EMesh* cube = engine->meshes[0];
		while(1){
				if(player){
					if(Collision::collision_of(vehicle,player)){
					std::cout << "collision\n";
					} 
					if(Collision::trigger_on(cube,player)){
						std::cout << "triiger\n";
					} 
				}
				
			
		}
		std::cout << "finish thread\n";
}

