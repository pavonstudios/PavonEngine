#ifndef _3D_OBJ_H_
#define _3D_OBJ_H_


#define GLM_FORCE_RADIANS

#include <unordered_map>
#include <vector>
#include <array>

#ifndef ANDROID
    #ifndef _OpenGL_Renderer_
        #include <vulkan/vulkan.h>
    #else
        #include <GL/gl.h>
    #endif
#else
    #include <GLES2/gl2.h>
    #include "glm/glm.hpp"
#endif

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"

#include "game_object.h"

#include "gltf.h"
#include "tiny_class.h"

namespace engine{
    struct Node;
    struct Skin {
        Node *skeleton_root = nullptr;
        std::vector<glm::mat4> inverse_bind_matrix;
        std::vector<Node*> joints;
    };
    struct Node{
        Node *parent;
        uint32_t index;
        glm::vec3 Translation{};
        glm::quat Rotation{};
        glm::mat4 matrix;
        std::vector<Node*> children;
        Skin* skin;
        int32_t skin_index = -1;
    };

   
}
struct UniformBufferObject {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};
struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;
    glm::vec4 joint0;
    glm::vec4 weight0;
#ifndef _OpenGL_Renderer_
    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription = {};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions = {};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

        return attributeDescriptions;
    }
#endif
    bool operator==(const Vertex& other) const {
        return pos == other.pos && color == other.color && texCoord == other.texCoord;
    }

};
#ifndef ANDROID

namespace std {
    template<> struct hash<Vertex> {
        size_t operator()(Vertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.texCoord) << 1);
        }
    };
}
#endif
struct image_size{
    int width;
    int heigth;
    void * pPixels;
};
/*
Load textures
*/
class AssetManager{
public:
    void free_image(void * pixels);
    image_size load_and_get_size(std::string texture_path);
};

namespace engine{
class EMesh : public GameObject{
public:
    ~EMesh();
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector< glm::vec3 > simple_vertices;
    tinygltf::Model gltf_model;
    void load_model(std::string model_path);

    glm::mat4 model_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::vec3 location_vector;
    std::string texture_path;

    std::vector<Node*> nodes;
    std::vector<Node*> linear_nodes;
    std::vector<Skin*> skins;

    void load_node(engine::Node *parent, const tinygltf::Node &gltf_node);
    void load_skins();
    Node* node_from_index(uint32_t index);
    Node* find_node(Node* parent, uint32_t index);

    virtual void SetLocation(float x, float y, float z);

    bool load_model2(const char * path);
#ifdef GLTF
    int load_model_gltf(const char* path);
    
#endif

#ifdef VULKAN
    VkBuffer vertices_buffer;
    VkBuffer indexBuffer;
    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<VkDescriptorSet> descriptorSets;
    VkDescriptorPool descriptorPool;
    VkPipeline graphics_pipeline;
    VkImage texture_image;
    VkImageView texture_image_view;
#endif
#ifdef _OpenGL_Renderer_
    GLuint vertexbuffer;
	GLuint VertexArrayID;
    GLuint indices_buffer;
#endif
    UniformBufferObject ubo;
		
};

class SkeletalMesh{
	public:
		
};

}//namespace
#endif