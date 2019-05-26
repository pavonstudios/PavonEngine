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
    #endif
    #ifdef WINDOWS
        #include <GL/gl.h>
    #endif
    #if defined(LINUX) && defined (ES2)
        #include <GLES2/gl2.h>
    #endif
	#ifdef DX11
		#include <d3d11.h>
		#include <DirectXMath.h>
		using namespace DirectX;
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
  
    struct Transform{
        vec3 location;
        quat rotation;
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

    struct PipelineData{
        std::string vertex_shader_path;
        std::string fragment_shader_path;
        #ifdef VULKAN
        VkPrimitiveTopology draw_type;
        #endif
        int mesh_type;
    };
           
    #include "collision_data.h"

}
struct UniformBufferObject {

	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;


  
#ifdef XM
	XMMATRIX model;
	XMMATRIX view;
	XMMATRIX projection;
#endif // DX11

};


#include "skeletal.h"

#include "texture.h"

namespace engine{


struct EMesh {
public:
    std::string name = "";
    int type = -1;
    int model_id = -1;
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

#ifdef DX11
	ID3D11Buffer* vertex_buffer;
	ID3D11Buffer* indices_buffer;
	ID3D11Buffer* uniform_buffer;

	ID3D11ShaderResourceView* shader_resource_view;
	ID3D11SamplerState* sampler_state;
	ID3D11Texture2D* dx_texture = NULL;
#endif 

    UniformBufferObject ubo; 
    PipelineData data_shader;
    glm::mat4 MVP;
    EImageData texture;
    Image image; 
    
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
            Engine* engine;
        #ifdef VULKAN
            vks::VulkanDevice* vulkan_device = nullptr;
            void clean_pipeline_from_meshes(std::vector<EMesh*> meshes);
        #endif
            int load_model_gltf(EMesh* mesh, const char* path);
            void load_primitives_data(EMesh* mesh, tinygltf::Model & gltf_model);
            void create_buffers(EMesh* mesh); 
            void create_buffers(Engine* engine, const std::vector<EMesh*> &meshes); 
            #ifdef ANDROID
            int load_mode_gltf_android(EMesh* mesh, const char* path, AAssetManager* pAssetManager);
            #endif
			#ifdef LINUX
            void load_textures_gltf(EMesh* mesh, tinygltf::Model & gltf_model);
			#endif	
            EMesh* mesh_by_name(std::string);
    };

    class TexturesManager{
        public:
            Engine* engine;
            void load_textures_to_cpu_memory(const std::vector<EMesh*> meshes);
            static void load_texture(Engine* engine, std::vector<EMesh*>& meshes);
    };

    class Objects{
    public:
        static void update_positions(Engine* engine, const TranslationUpdate &translation);
        static void translate(Engine* engine, EMesh* mesh, Movement& movement);
    };



}//namespace engine

#endif