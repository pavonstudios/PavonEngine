#include "animation_manager.hpp"
#include "engine.h"


void AnimationManager::play_animations(Engine* engine){
	if(engine->play_animations){
			engine->animation_time += engine->deltaTime;
			std::vector<SkeletalMesh*> skeletals;
			skeletals.push_back(engine->skeletal_meshes[0]->skeletal);
			//skeletals.push_back(skeletal_meshes[1]->skeletal);
			Skeletal::play_animations(skeletals,engine->animation_time);

			std::cout << "time: " << engine->animation_time << std::endl;
			if(engine->animation_time >= 3){
				engine->animation_time = 0;
				//Skeletal::reset_animations(skeletals);
				//play_animations = false;
			}
		}
}