#ifndef ANIMATION_MANAGER
#define ANIMATION_MANAGER
class Engine;
#include "skeletal.h"
#include <vector>


using namespace engine;
class AnimationManager{
	public:
		void play_animations(Engine* engine);
		std::vector<Animation*> animations;
		static void load_animation(SkeletalMesh* skeletal, tinygltf::Model &gltf_model);}
		

};

#endif