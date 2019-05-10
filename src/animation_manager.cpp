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

void AnimationManager::load_animation(SkeletalMesh* skeletal, tinygltf::Model &gltf_model){
    for(auto& anim : gltf_model.animations){
        Animation new_animation;

        for(auto& sampler : anim.samplers){
            AnimationSampler new_sampler{};

            {
                //inputs
                const tinygltf::Accessor &input_accessor = gltf_model.accessors[sampler.input];
                const tinygltf::BufferView &input_bufferView = gltf_model.bufferViews[input_accessor.bufferView];
                const tinygltf::Buffer &input_buffer = gltf_model.buffers[input_bufferView.buffer];

                
                const void *dataPtr = &input_buffer.data[input_accessor.byteOffset + input_bufferView.byteOffset];
                const float *buf = static_cast<const float*>(dataPtr);
                for (size_t index = 0; index < input_accessor.count; index++) {
                    new_sampler.inputs.push_back(buf[index]);
                }
            }


            //outputs
            const tinygltf::Accessor &accessor = gltf_model.accessors[sampler.output];
            const tinygltf::BufferView &bufferView = gltf_model.bufferViews[accessor.bufferView];
            const tinygltf::Buffer &buffer = gltf_model.buffers[bufferView.buffer];

            const void *dataPtr = &buffer.data[accessor.byteOffset + bufferView.byteOffset];

            switch (accessor.type) {
                case TINYGLTF_TYPE_VEC3: {
                    const glm::vec3 *buf = static_cast<const glm::vec3*>(dataPtr);
                    for (size_t index = 0; index < accessor.count; index++) {
                        new_sampler.outputs_vec4.push_back(glm::vec4(buf[index], 0.0f));
                    }
                    break;
                }
                case TINYGLTF_TYPE_VEC4: {
                    const glm::vec4 *buf = static_cast<const glm::vec4*>(dataPtr);
                    for (size_t index = 0; index < accessor.count; index++) {
                        new_sampler.outputs_vec4.push_back(buf[index]);
                    }
                    break;
                }
                default: {
                    std::cout << "unknown type" << std::endl;
                    break;
                }
            }
             //channels
            for(auto& source : anim.channels){
                AnimationChannel channel{};
                channel.sampler_index = source.sampler;
                channel.node = Skeletal::node_from_index(skeletal->mesh,source.target_node);
                channel.node_index = source.target_node;
                
                if(source.target_path == "rotation"){
                    channel.PathType = PATH_TYPE_ROTATION;
                }

                if(source.target_path == "translation"){
                    channel.PathType = PATH_TYPE_TRANSLATION;
                }
                
                new_animation.channels.push_back(channel);
            }

            new_animation.samplers.push_back(new_sampler);

        }        

        skeletal->animations.push_back(new_animation);
    }
}