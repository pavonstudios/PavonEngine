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
#ifdef ANDROID
    #include "glm/glm.hpp"
    #include "glm/gtc/matrix_transform.hpp"
    #include "glm/gtc/type_ptr.hpp"
#else
    #include <glm/glm.hpp>
    #include <glm/gtc/matrix_transform.hpp>
    #include <glm/gtc/type_ptr.hpp>    
#endif

#include "tiny_class.h"
#include "vertex.h"

class Engine;


#define MESH_TYPE_STATIC 0
#define MESH_TYPE_SKINNED 1
#define MESH_WITH_COLLIDER 5
#define MESH_LOD 8

#define RIGTH   1
#define LEFT    2
#define UP      3
#define DOWN    4
#define FORWARD    5
#define BACKWARD    6

using namespace glm;

namespace engine{
    struct Trigger{
        bool collision = false;
        bool positive_x = false;
        bool negative_x = false;
        bool positive_y = false;
        bool negative_y = false;
    };
    
    struct MapDataToLoad
	{
        std::string model_path;
        std::string texture_path;
        glm::vec3 location;
        int type;
        int texture_id = -1;
        int model_id = -1;
	};

    struct Movement{
        vec3 direction;
        float value;
    };       

    struct Collider{
        bool collision = false;
        bool positive_x = false;
        bool negative_x = false;
        bool positive_y = false;
        bool negative_y = false;
        bool can_move_negative_x = true;
        bool can_move_positive_x = true;
        bool can_move_negative_y = true;
        bool can_move_positive_y = true;
    };

    struct AABB{
        glm::vec3 m_vecMax;
        glm::vec3 m_vecMin;
    };
    struct EImageData{
        unsigned char* data;
        int size = 0;
        bool hasTexture = false;
        float height = 0;
        float width = 0;
        #ifdef VULKAN
        VkFormat format;
        #endif
        int texture_id = -1;
    };
    struct pipeline_data{
        std::string vertex_shader_path;
        std::string fragment_shader_path;
        #ifdef VULKAN
        VkPrimitiveTopology draw_type;
        #endif
        int mesh_type;
    };
    
    
    
    struct NodeUniform{
        alignas(16) glm::mat4 matrix;
        alignas(16) glm::mat4 joint_matrix[125];
        float joint_count;
    };
   
}
struct UniformBufferObject {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};


#include "skeletal.h"

namespace engine{


struct EMesh {
public:
    int type = -1;
    int model_id = -1;
    std::string name;
    EMesh();
#ifdef VULKAN
    EMesh(vks::VulkanDevice* vulkan_device);
    vks::VulkanDevice* vulkan_device;
    void init(vks::VulkanDevice* vulkan_device);
#endif
    ~EMesh();
    void clean_object();

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
    GLuint vertex_buffer = -1;
	GLuint indices_buffer = -1;
    GLuint shader_program = -1;
    GLuint texture_id = -1;
#endif

    UniformBufferObject ubo; 
    pipeline_data data_shader;
    glm::mat4 MVP;
    EImageData texture; 
    
    glm::mat4 model_matrix = glm::mat4(1.0);
    glm::vec3 location_vector;
    std::string texture_path; 

    AABB box;
    Collider collider;

    bool bIsGUI = false;

    EMesh* lod1 = nullptr;

    #ifdef GLTF   

    SkeletalMesh* skeletal = nullptr;

    std::vector<Node*> nodes;
    std::vector<Node*> linear_nodes;
    std::vector<Skin*> skins;
    struct NodeUniform node_uniform;  
    
    tinygltf::Model gltf_model;
               
    #endif
    
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
		
};

    struct TranslationUpdate{
            std::vector<EMesh*> meshes;
            std::vector<Movement> movements;
    };

    struct Model{
        EMesh* mesh;
        #if defined(ES2) || defined(ANDROID)   
        GLuint shader_program = -1;
        GLuint texture_id = -1;
        #endif
    };


    class MeshManager{
        public:
        #ifdef VULKAN
            vks::VulkanDevice* vulkan_device;
            void clean_pipeline_from_meshes(std::vector<EMesh*> meshes);
        #endif
            int load_model_gltf(EMesh* mesh, const char* path);
            void load_primitives_data(EMesh* mesh, tinygltf::Model & gltf_model);
            void create_buffers(EMesh* mesh); 
            void create_buffers(Engine* engine, const std::vector<EMesh*> &meshes); 
            #ifdef ANDROID
            int load_mode_gltf_android(EMesh* mesh, const char* path, AAssetManager* pAssetManager);
            #endif   
            void load_textures_gltf(EMesh* mesh, tinygltf::Model & gltf_model);
    };

    class Objects{
    public:
        static void update_positions(Engine* engine, const TranslationUpdate &translation);
        static void translate(Engine* engine, EMesh* mesh, Movement& movement);
    };



}//namespace engine

#endif