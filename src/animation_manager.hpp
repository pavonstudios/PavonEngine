#ifndef ANIMATION_MANAGER
#define ANIMATION_MANAGER
class Engine;
#include "skeletal.h"
#include <vector>


using namespace engine;
class AnimationManager{
	public:
		Engine* engine;
		void play_animations(Engine* engine);
		std::vector<Animation*> animations;
		void load_animation(SkeletalMesh* skeletal, tinygltf::Model &gltf_model);
		Animation* animation_by_name(std::string name);
		void play_animation(SkeletalMesh* skeletal, std::string name);

};

#endif