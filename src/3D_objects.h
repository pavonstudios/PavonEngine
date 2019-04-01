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

#include "glm/gtx/hash.hpp"

#include "game_object.h"

struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;
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
class AssetManager{
public:
    void free_image(void * pixels);
    image_size load_and_get_size(std::string texture_path);
};
namespace engine{
class Mesh : public GameObject{
public:
    
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector< glm::vec3 > simple_vertices;

    void load_model(std::string model_path);

    glm::mat4 model_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::vec3 location_vector;
    

    virtual void SetLocation(float x, float y, float z);

    bool load_model2(const char * path);

    int load_model_gltf(const char* path);


#ifndef _OpenGL_Renderer_
    VkBuffer vertices_buffer;
    VkBuffer indexBuffer;
    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<VkDescriptorSet> descriptorSets;
    VkDescriptorPool descriptorPool;
#endif
#ifdef _OpenGL_Renderer_
    GLuint vertexbuffer;
	GLuint VertexArrayID;
#endif

		
};

}//namespace
#endif