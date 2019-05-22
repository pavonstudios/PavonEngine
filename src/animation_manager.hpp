#ifndef ANIMATION_MANAGER
#define ANIMATION_MANAGER
class Engine;
#include "skeletal.h"
#include <vector>
#include <list>


using namespace engine;


struct AnimationPlay{
	Animation* animation;
	SkeletalMesh* skeletal;
};

class AnimationManager{
	public:
		SkeletalLoader* skeletal_loader = nullptr;
		std::list<AnimationPlay*> animation_to_play;
		AnimationManager();
		~AnimationManager();
		void clear_loaders();
		Engine* engine;
		void play_animations(Engine* engine);
		std::vector<Animation*> animations;
		void load_animation(SkeletalMesh* skeletal, tinygltf::Model &gltf_model);
		Animation* animation_by_name(std::string name);
		void play_animation(SkeletalMesh* skeletal, std::string name, bool loop);
		void play(Animation*,SkeletalMesh*);
		void add_to_queue(SkeletalMesh* skeletal, std::string name, bool loop);
		bool work_animation_play(AnimationPlay* play);
};

#endif