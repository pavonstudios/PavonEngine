#ifndef _3D_OBJ_H_
#define _3D_OBJ_H_

#include <unordered_map>
#include <vector>
#include <array>
#include <iostream>
#include <cstring>

#ifndef ANDROID
    #ifdef VULKAN
        #include <vulkan/vulkan.h>
        #include "VulkanDevice.hpp"
    #else
        #include <GLES2/gl2.h>
    #endif
    #include <glm/gtc/quaternion.hpp>
#else
    #include <GLES2/gl2.h>
    #include "glm/glm.hpp"
    #include "glm/gtc/quaternion.hpp"
    #include <android/asset_manager.h>
#endif

#include "game_object.h"

#include "tiny_class.h"
#include "vertex.h"



#define MESH_TYPE_STATIC 0
#define MESH_TYPE_SKINNED 1

namespace engine{
    struct EImageData{
        unsigned char* data;
        int size = 0;
        bool hasTexture = false;
        float height = 0;
        float width = 0;
        #ifdef VULKAN
        VkFormat format;
        #endif
    };
    struct pipeline_data{
        std::string vertex_shader_path;
        std::string fragment_shader_path;
        #ifdef VULKAN
        VkPrimitiveTopology draw_type;
        #endif
        int mesh_type;
    };
    
    struct Node;
    struct Skin {
        Node *skeleton_root = nullptr;
        std::vector<glm::mat4> inverse_bind_matrix;
        std::vector<Node*> joints;
    };
    
    struct NodeUniform{
        alignas(16) glm::mat4 matrix;
        alignas(16) glm::mat4 joint_matrix[128];
        float joint_count;;
    };
   
}
struct UniformBufferObject {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};



namespace engine{
class EMesh : public GameObject{
public:
    EMesh();
#ifdef VULKAN
    EMesh(vks::VulkanDevice* vulkan_device);
    vks::VulkanDevice* vulkan_device;
#endif
    ~EMesh();
    void create_buffers();
    void clean_object();
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
    glm::mat4 model_matrix = glm::mat4(1.0);
    glm::vec3 location_vector;
    std::string texture_path;   

    //skeletal
    void load_joints_matrix();
    void load_node(engine::Node *parent, uint32_t index, const tinygltf::Node &gltf_node);
    void load_skins();
    Node* node_from_index(uint32_t index);
    Node* find_node(Node* parent, uint32_t index);

    virtual void SetLocation(float x, float y, float z);    

#ifdef GLTF
    int load_model_gltf(const char* path);
    #ifdef ANDROID
    int load_mode_gltf_android(const char* path, AAssetManager* pAssetManager);
    #endif
    void load_primitives_data();
    void load_textures_gltf();
    std::vector<Node*> nodes;
    std::vector<Node*> linear_nodes;
    std::vector<Skin*> skins;
    tinygltf::Model gltf_model;
    struct NodeUniform node_uniform;             
#endif

#ifdef VULKAN
    VkBuffer vertices_buffer;
    VkBuffer indexBuffer;
    VkBuffer node_unifrom_buffer;
    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<VkDescriptorSet> descriptorSets;
    VkDescriptorPool descriptorPool;
    VkPipeline graphics_pipeline;
    VkImage texture_image;
    VkImageView texture_image_view;

    std::vector<VkBuffer> uniform_node_buffers;
    std::vector<VkDeviceMemory> uniform_node_buffer_memory;
    

    VkDeviceMemory vertexBufferMemory;
    VkDeviceMemory indexBufferMemory;
    VkDeviceMemory textureImageMemory;   
#endif
#if defined(ES2) || defined(ANDROID)
    GLuint vertex_buffer;
	GLuint indices_buffer;
    GLuint shader_program;
    GLuint texture_id;
#endif
    UniformBufferObject ubo;
    pipeline_data data;
    glm::mat4 MVP;
    EImageData texture;
		
};
#include "Node.h"
}//namespace engine
#endif