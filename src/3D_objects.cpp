
#include "3D_objects.h"
#include "iostream"

#ifdef ANDROID
    #include "glm/glm.hpp"
    #include "glm/gtc/matrix_transform.hpp"
    #include "glm/gtc/type_ptr.hpp"
#else
    #include <glm/glm.hpp>
    #include <glm/gtc/matrix_transform.hpp>
    #include <glm/gtc/type_ptr.hpp>    
#endif
#ifdef ES2
    #include <GLES2/gl2.h>
#endif

#ifdef GLTF
using namespace engine;

void EMesh::create_buffers(){
    #ifdef ES2
        glGenBuffers(1,&vertex_buffer);
        glBindBuffer(GL_ARRAY_BUFFER,vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER,vertices.size() * sizeof(Vertex),vertices.data(),GL_STATIC_DRAW);

        glGenBuffers(1,&indices_buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,indices_buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size() * sizeof(unsigned int),indices.data(), GL_STATIC_DRAW);

    #endif
}
void EMesh::load_joints_matrix(){
    
}
Node* EMesh::find_node(Node* parent, uint32_t index){
    Node* node_found = nullptr;
    if(parent->index == index)
        return parent;
    for(auto& child : parent->children){
        node_found = find_node(child,index);
        if(node_found)
            break;
    }
    return node_found;
}

Node* EMesh::node_from_index(uint32_t index){
    Node* node_found = nullptr;
    for(auto &node : nodes){
        node_found = find_node(node,index);
        if(node_found)
            break;
    }
    return node_found;
}
void EMesh::load_skins(){
    for(tinygltf::Skin &source_skin: gltf_model.skins){
        Skin *new_skin = new Skin{};
        if(source_skin.skeleton > -1){
            new_skin->skeleton_root = node_from_index(source_skin.skeleton);
        }
        for(int joint_index : source_skin.joints){
            Node* node = node_from_index(joint_index);
            if(node)
                new_skin->joints.push_back(node);
        }
        if(source_skin.inverseBindMatrices > -1){
            const tinygltf::Accessor &accessor = gltf_model.accessors[source_skin.inverseBindMatrices];

            const tinygltf::BufferView &bufferView = gltf_model.bufferViews[accessor.bufferView];
            
            const tinygltf::Buffer &buffer = gltf_model.buffers[bufferView.buffer];
            
            new_skin->inverse_bind_matrix.resize(accessor.count);
            
            memcpy(new_skin->inverse_bind_matrix.data(),
                     &buffer.data[accessor.byteOffset + bufferView.byteOffset],
                     accessor.count * sizeof(glm::mat4));

        }

        skins.push_back(new_skin);
    }
    
}
void EMesh::load_textures_gltf(){
    if(gltf_model.images.size() > 0){
        tinygltf::Image tiny_image = gltf_model.images[0];

        unsigned char * image_data = nullptr;
        u_int32_t image_size = 0;

        image_data = &tiny_image.image[0];
        image_size = tiny_image.image.size();
    }
    
}

void EMesh::load_node(engine::Node *parent, uint32_t index, const tinygltf::Node &gltf_node){
    Node *new_node = new Node{};
    new_node->parent = parent;
    new_node->matrix = glm::mat4(1.0f);
    new_node->skin_index = gltf_node.skin;
    new_node->index = index;
    new_node->name = gltf_node.name;
    
    //some nodes do not contain transform information
    if(gltf_node.translation.size() == 3)
        new_node->Translation = glm::make_vec3(gltf_node.translation.data());

    if(gltf_node.rotation.size() == 4)
        new_node->Rotation = glm::make_quat(gltf_node.rotation.data());

    if(gltf_node.matrix.size() == 16)
        new_node->matrix = glm::make_mat4x4(gltf_node.matrix.data());

    int children_count = gltf_node.children.size();

    if( children_count > 0){
        for(size_t i = 0;i < children_count ;i++){
            load_node(new_node,gltf_node.children[i],gltf_model.nodes[gltf_node.children[i]]);
        }
    }
    if(gltf_node.mesh > -1){
        new_node->mesh = this;
    }
    if(parent){
        parent->children.push_back(new_node);       
    }else{
         nodes.push_back(new_node);
    }

    linear_nodes.push_back(new_node);
}
#ifdef ANDROID
int EMesh::load_mode_gltf_android(const char* path, AAssetManager* assetManager){
    
    
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    bool ret = loader.LoadASCIIFromFileAndroid(&gltf_model, &err, &warn, path,0,assetManager);
    //bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, argv[1]); // for binary glTF(.glb)
    if(ret){
        load_primitives_data();
        return 1;
    }
    
    return -1;
}
#endif//android gltf loader
int EMesh::load_model_gltf(const char* path){
    
    
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    bool ret = loader.LoadASCIIFromFile(&gltf_model, &err, &warn, path);
    //bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, argv[1]); // for binary glTF(.glb)
    if(!ret){
        std::string error = "Failed to open " + std::string(path);
        #ifndef ANDROID
        throw std::runtime_error(error);
        return 0;
        #else
        std::cout << "error inlcoad file " << std::endl;
        return -1;
        #endif

    }
    
    load_primitives_data();
    load_textures_gltf();

    int node_count = gltf_model.nodes.size();
    for(size_t i = 0; i < node_count;i++){
        load_node(nullptr,i,gltf_model.nodes[i]);
    }

    load_skins();
    for(auto node : linear_nodes){
       // if(node->skin_index > -1)
         //   node->skin = skins[node->skin_index];
        if(node->mesh){
            if(skins.size()>0)
                node->skin = skins[0];
           //node->update(); //for some reason this not work, produce issues in vertices transformation
        }
    }   
    //linear_nodes[4]->skin = skins[0];
    //linear_nodes[4]->update();


    return 1;
}

void EMesh::load_primitives_data(){
    for(int i = 0; i < gltf_model.meshes.size();i++){
        for(auto primitive : gltf_model.meshes[i].primitives){
            uint32_t indexStart = static_cast<uint32_t>(indices.size());
            uint32_t vertexStart = static_cast<uint32_t>(vertices.size());
            uint32_t indexCount = 0;
            uint32_t vertexCount = 0;
            const float *bufferPos = nullptr;
            const uint16_t *bufferJoints = nullptr;
            const float *bufferWeights = nullptr;
            const float *bufferTexCoordSet0 = nullptr;

            const tinygltf::Accessor &posAccessor = gltf_model.accessors[primitive.attributes.find("POSITION")->second];
            const tinygltf::BufferView &posView = gltf_model.bufferViews[posAccessor.bufferView];
            bufferPos = reinterpret_cast<const float *>(&(gltf_model.buffers[posView.buffer].data[posAccessor.byteOffset + posView.byteOffset]));
            vertexCount = static_cast<uint32_t>(posAccessor.count);

            if (primitive.attributes.find("TEXCOORD_0") != primitive.attributes.end()) {
                const tinygltf::Accessor &uvAccessor = gltf_model.accessors[primitive.attributes.find("TEXCOORD_0")->second];
                const tinygltf::BufferView &uvView = gltf_model.bufferViews[uvAccessor.bufferView];
                bufferTexCoordSet0 = reinterpret_cast<const float *>(&(gltf_model.buffers[uvView.buffer].data[uvAccessor.byteOffset + uvView.byteOffset]));
            }
            // Skinning
            // Joints
            if (primitive.attributes.find("JOINTS_0") != primitive.attributes.end()) {
                const tinygltf::Accessor &jointAccessor = gltf_model.accessors[primitive.attributes.find("JOINTS_0")->second];
                const tinygltf::BufferView &jointView = gltf_model.bufferViews[jointAccessor.bufferView];
                bufferJoints = reinterpret_cast<const uint16_t *>(&(gltf_model.buffers[jointView.buffer].data[jointAccessor.byteOffset + jointView.byteOffset]));
            }
            if (primitive.attributes.find("WEIGHTS_0") != primitive.attributes.end()) {
                const tinygltf::Accessor &uvAccessor = gltf_model.accessors[primitive.attributes.find("WEIGHTS_0")->second];
                const tinygltf::BufferView &uvView = gltf_model.bufferViews[uvAccessor.bufferView];
                bufferWeights = reinterpret_cast<const float *>(&(gltf_model.buffers[uvView.buffer].data[uvAccessor.byteOffset + uvView.byteOffset]));
            }

            bool has_skin = (bufferJoints && bufferWeights);
            for(size_t c = 0; c < posAccessor.count; c++){
                Vertex vert{};
                vert.pos = glm::vec4(glm::make_vec3(&bufferPos[c * 3]), 1.0f);
                vert.texCoord = bufferTexCoordSet0 ? glm::make_vec2(&bufferTexCoordSet0[c * 2]) : glm::vec3(0.0f);
                vert.joint0 = has_skin ? glm::vec4(glm::make_vec4(&bufferJoints[c * 4])) : glm::vec4(0.0f);
                vert.weight0 = has_skin ? glm::make_vec4(&bufferWeights[c * 4]) : glm::vec4(0.0f);


                vertices.push_back(vert);
            }

            //indices

            const tinygltf::Accessor &accessor = gltf_model.accessors[primitive.indices > -1 ? primitive.indices : 0];
            const tinygltf::BufferView &bufferView = gltf_model.bufferViews[accessor.bufferView];
            const tinygltf::Buffer &buffer = gltf_model.buffers[bufferView.buffer];

            indexCount = static_cast<uint32_t>(accessor.count);
            const void *dataPtr = &(buffer.data[accessor.byteOffset + bufferView.byteOffset]);

            switch (accessor.componentType) {
                case TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT: {
                    const uint32_t *buf = static_cast<const uint32_t*>(dataPtr);
                    for (size_t index = 0; index < accessor.count; index++) {
                        indices.push_back(buf[index] + vertexStart);
                    }
                    break;
                }
                case TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT: {
                    const uint16_t *buf = static_cast<const uint16_t*>(dataPtr);
                    for (size_t index = 0; index < accessor.count; index++) {
                        indices.push_back(buf[index] + vertexStart);
                    }
                    break;
                }
                case TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE: {
                    const uint8_t *buf = static_cast<const uint8_t*>(dataPtr);
                    for (size_t index = 0; index < accessor.count; index++) {
                        indices.push_back(buf[index] + vertexStart);
                    }
                    break;
                }
                default:
                    std::cerr << "Index component type " << accessor.componentType << " not supported!" << std::endl;
                    //return 2;
                }
        }//end loop primitives
    }


}
#endif

using namespace engine;
#ifdef VULKAN
EMesh::EMesh(vks::VulkanDevice* vulkan_device){
    this->vulkan_device = vulkan_device;
    this->node_uniform.matrix = glm::mat4(1.0);
        

    VkDeviceSize bufferSize = sizeof(NodeUniform);

    uniform_node_buffers.resize(3);
    uniform_node_buffer_memory.resize(3);

    for (size_t i = 0; i < 3; i++) {
        vulkan_device->createBuffer(
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            sizeof(NodeUniform),
            &uniform_node_buffers[i],
            &uniform_node_buffer_memory[i],
            &node_uniform);               
    }
}
EMesh::~EMesh(){
    clean_object();
        for(auto buffer: uniformBuffers){
            vkDestroyBuffer(vulkan_device->logicalDevice,buffer,nullptr);
        }
        for(auto buffer: uniform_node_buffers){
            vkDestroyBuffer(vulkan_device->logicalDevice,buffer,nullptr);
        }
          vkDestroyBuffer(vulkan_device->logicalDevice,indexBuffer,nullptr);
        vkDestroyBuffer(vulkan_device->logicalDevice,vertices_buffer,nullptr);
         for(auto uniform_memory : uniformBuffersMemory){
            vkFreeMemory(vulkan_device->logicalDevice,uniform_memory, nullptr);
        }
        for(auto uniform_memory : uniform_node_buffer_memory){
            vkFreeMemory(vulkan_device->logicalDevice,uniform_memory, nullptr);
        }
         vkFreeMemory(vulkan_device->logicalDevice, textureImageMemory, nullptr);
        vkFreeMemory(vulkan_device->logicalDevice, indexBufferMemory, nullptr);
        vkFreeMemory(vulkan_device->logicalDevice, vertexBufferMemory, nullptr);
}
#else
EMesh::EMesh(){
    
}
EMesh::~EMesh(){

}
#endif
void EMesh::clean_object(){
    #ifdef VULKAN
        if(graphics_pipeline != VK_NULL_HANDLE)
            vkDestroyPipeline(vulkan_device->logicalDevice,graphics_pipeline,nullptr);  
       
    #endif
}


void EMesh::SetLocation(float x, float y, float z){
		Location.x = x;
		Location.y = y;
		Location.z = z;

        glm::mat4 myMatrix = glm::translate(glm::mat4(), glm::vec3(Location.x,Location.y,Location.z));
        glm::vec4 myVector(10.0f, 10.0f, 10.0f, 0.0f);
        glm::vec4 transformedVector = myMatrix * myVector; // guess the result
       // model_matrix = transformedVector;
       model_matrix = glm::translate(model_matrix, glm::vec3(Location.x,Location.y,Location.z));
}


