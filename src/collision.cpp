#include "engine.h"
#include "collision.h"
#include "Game/game.hpp"
using namespace engine;


void Collision::create_collision_helper_vertices(Engine* engine){
	for(EMesh* mesh_with_collider : engine->linear_meshes){
		if(mesh_with_collider->type == MESH_WITH_COLLIDER){
			EMesh* mesh_collider_helper = new EMesh();
			vec3 max = mesh_with_collider->box.m_vecMax;
			vec3 min = mesh_with_collider->box.m_vecMin;
			vec3 world_position = mesh_with_collider->location_vector;


			Vertex vert1 {};
			vert1.pos = vec3(max.x + world_position.x, max.y + world_position.y , max.z + world_position.z);

			Vertex vert2 {};
			vert2.pos = vec3(min.x+world_position.x, max.y+world_position.y, max.z+world_position.z);

			Vertex vert3 {};
			vert3.pos = vec3(min.x+world_position.x, min.y+world_position.y , max.z+world_position.z);

			Vertex vert4 {};
			vert4.pos = vec3(min.x+world_position.x, min.y+world_position.y , min.z+world_position.z);

			Vertex vert5 {};
			vert5.pos = vec3(max.x+world_position.x, max.y+world_position.y , min.z+world_position.z);
			
			Vertex vert6 {};
			vert6.pos = vec3(max.x+world_position.x, min.y+world_position.y , min.z+world_position.z);

			Vertex vert7 {};
			vert7.pos = vec3(max.x+world_position.x, min.y+world_position.y , max.z+world_position.z);

			Vertex vert8 {};
			vert8.pos = vec3(min.x+world_position.x, max.y+world_position.y , min.z+world_position.z);


			mesh_collider_helper->vertices.push_back(vert1);
			mesh_collider_helper->vertices.push_back(vert2);
			mesh_collider_helper->vertices.push_back(vert3);
			mesh_collider_helper->vertices.push_back(vert4);
			mesh_collider_helper->vertices.push_back(vert5);
			mesh_collider_helper->vertices.push_back(vert6);
			mesh_collider_helper->vertices.push_back(vert7);
			mesh_collider_helper->vertices.push_back(vert8);

			mesh_collider_helper->data_shader.fragment_shader_path = "Game/Assets/shaders/gles/green.glsl";

			
			mesh_collider_helper->data_shader.vertex_shader_path = "Game/Assets/shaders/gles/vert_mvp.glsl";
		
			mesh_collider_helper->type = -1;
			mesh_collider_helper->bIsGUI = false;
			mesh_collider_helper->name = "collider";   


			engine->linear_meshes.push_back(mesh_collider_helper);
			engine->colliders_draw_mesh.push_back(mesh_collider_helper);

		}
	}
}

bool Collision::sphere_collision_of(EMesh* mesh2 , EMesh* mesh){

		bool y = false;
		bool z = false;
		bool x = false;

		//AABB tBox = mesh2->box;
		mesh->collider.collision = false;
		mesh->collider.positive_x = false;
		mesh->collider.negative_x = false;
		mesh->collider.positive_y = false;
		mesh->collider.negative_y = false;

		//glm::vec3 vecPoint = mesh->location_vector;
		//glm::vec3 mesh2_vec = mesh2->location_vector;

		
		float distance = glm::distance(mesh->location_vector, mesh2->collider.sphere->center);
		float sum_of_radius = mesh->collider.sphere->radius + mesh2->collider.sphere->radius;
		
		if(distance < sum_of_radius){
			x =true;
			y = true;
			z = true;//TODO:
		}
	
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

		//AABB tBox = mesh2->box;
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

	//	AABB tBox = mesh2->box;
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

		EMesh* other_sphere = engine->meshes[11];

		player->collider.sphere = new SphereCollider;
		player->collider.sphere->radius = 0.5;
		player->collider.sphere->center = player->location_vector;

		other_sphere->collider.sphere = new SphereCollider;
		other_sphere->collider.sphere->radius = 0.5;
		other_sphere->collider.sphere->center = other_sphere->location_vector;


		while(1){
				if(player){
					if( Collision::collision_of(vehicle,player) ){
					std::cout << "AABB collision\n";
					} 
					if( Collision::trigger_on(cube,player) ){
						std::cout << "triiger\n";
					} 
					if( Collision::sphere_collision_of(other_sphere,player) ){
						std::cout << "sphere collision\n";
					}
				}
				
			
		}
		std::cout << "finish thread\n";
}

