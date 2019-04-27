
#include "objects.h"
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
EMesh::EMesh(){
    
}

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

void EMesh::init(vks::VulkanDevice* vulkan_device){
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
#endif
EMesh::~EMesh(){
    #ifdef VULKAN
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
    #endif
}
void MeshManager::create_buffers(EMesh* mesh){
    #if defined(ES2) || defined(ANDROID)
        glGenBuffers(1,&mesh->vertex_buffer);
        glBindBuffer(GL_ARRAY_BUFFER,mesh->vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER,mesh->vertices.size() * sizeof(Vertex),mesh->vertices.data(),GL_STATIC_DRAW);

        if(mesh->indices.size() > 0){
                glGenBuffers(1,&mesh->indices_buffer);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,mesh->indices_buffer);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER,mesh->indices.size() * sizeof(unsigned int),mesh->indices.data(), GL_STATIC_DRAW);

        }
        
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

    #endif
}

void EMesh::load_textures_gltf(){
    EImageData image_data_struct = {};
    if(gltf_model.images.size() > 0){
        tinygltf::Image tiny_image = gltf_model.images[0];

        unsigned char * image_data = nullptr;
        u_int32_t image_size = 0;

        if(tiny_image.component == 3){
            std::cout << "RGB\n"; 
        }


        image_data = &tiny_image.image[0];
        image_size = tiny_image.image.size();
        std::cout << "image loaded from gltf loader\n";

        image_data_struct.data = image_data;
        image_data_struct.size = image_size;
        image_data_struct.hasTexture = true;
        image_data_struct.height = tiny_image.height;
        image_data_struct.width = tiny_image.width;
        #ifdef VULKAN
        image_data_struct.format = VK_FORMAT_R8G8B8A8_UNORM;
        #endif


        this->texture = image_data_struct;
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



#endif





void EMesh::clean_object(){
    #ifdef VULKAN
        if(graphics_pipeline != VK_NULL_HANDLE)
            vkDestroyPipeline(vulkan_device->logicalDevice,graphics_pipeline,nullptr);  
       
    #endif
}


void Skeletal::load_node(EMesh* mesh, node_load_data& node_data) {
    Node *new_node = new Node{};
    new_node->parent = node_data.parent;
    new_node->matrix = glm::mat4(1.0f);
    new_node->skin_index = node_data.gltf_node->skin;
    new_node->index = node_data.index;
    new_node->name = node_data.gltf_node->name;
    
    //some nodes do not contain transform information
    if(node_data.gltf_node->translation.size() == 3)
        new_node->Translation = glm::make_vec3(node_data.gltf_node->translation.data());

    if(node_data.gltf_node->rotation.size() == 4)
        new_node->Rotation = glm::make_quat(node_data.gltf_node->rotation.data());

    if(node_data.gltf_node->matrix.size() == 16)
        new_node->matrix = glm::make_mat4x4(node_data.gltf_node->matrix.data());

    int children_count = node_data.gltf_node->children.size();
   

    if( children_count > 0){
        for(size_t i = 0;i < children_count ;i++){
            //load_node(new_node,gltf_node.children[i],gltf_model.nodes[gltf_node.children[i]]);
        }
    }
    if(node_data.gltf_node->mesh > -1){
        new_node->mesh = mesh;
    }
}

int MeshManager::load_model_gltf(EMesh* mesh, const char* path){    
    
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    bool ret = loader.LoadASCIIFromFile(&mesh->gltf_model, &err, &warn, path);
    //bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, argv[1]); // for binary glTF(.glb)
    if(!ret){
        std::string error = "Failed to open " + std::string(path);
        #ifndef ANDROID
            std::cout << "ERROR: " << err << std::endl;
            throw std::runtime_error(error);
            return -1;
        #else
            std::cout << "error inlcoad file " << std::endl;
            return -1;
        #endif

    }   
   
    load_primitives_data(mesh, mesh->gltf_model);
    mesh->load_textures_gltf();    

    return 1;
}

void MeshManager::load_skeletal_data(EMesh* mesh){
    int node_count = mesh->gltf_model.nodes.size();
    for(size_t i = 0; i < node_count;i++){
        mesh->load_node(nullptr,i,mesh->gltf_model.nodes[i]);
    }

    Skeletal::load_skin(mesh, mesh->gltf_model);
    bool isUpdated = false;
    Node* node_with_mesh;
    for(auto node : mesh->linear_nodes){
        if(node->mesh){
            if(mesh->skins.size()>0){
                node->skin = mesh->skins[0];

                if(!isUpdated){
                //NodeManager::update(node);
               // isUpdated = true;
                }
            //for some reason this not work, produce issues in vertices transformation
                node_with_mesh = node;
            }
                
        }
    }     
    //mesh->model_matrix = glm::rotate(mesh->model_matrix,glm::radians(90.0f),glm::vec3(1,0,0));  
    update_joints_matrix(mesh, node_with_mesh);
    for(Node* node : mesh->linear_nodes){
        if(node->name == "lower_arm"){
            NodeManager::update(mesh, node);
        }
    }
}

void MeshManager::update_joints_matrix(EMesh* mesh, Node* node){
    mesh->node_uniform.matrix = glm::mat4(1.0);
    mesh->node_uniform.joint_count = 4;
    mesh->node_uniform.joint_matrix[0] = glm::translate(glm::mat4(1.0),glm::vec3(0,0,0));
    mesh->node_uniform.joint_matrix[1] = glm::translate(glm::mat4(1.0),glm::vec3(0,0,0));
    mesh->node_uniform.joint_matrix[2] = glm::translate(glm::mat4(1.0),glm::vec3(0,0,0));
    mesh->node_uniform.joint_matrix[3] = glm::translate(glm::mat4(1.0),glm::vec3(0,0,0));

    glm::mat4 rot = glm::rotate(glm::mat4(1.0),glm::radians(15.f),glm::vec3(0,1,0));
    mesh->node_uniform.joint_matrix[2] = mesh->node_uniform.joint_matrix[2] * rot;
    //mesh->node_uniform.joint_matrix[3] = 
    //glm::mat4 global_inverse = glm::inverse(NodeManager::get_global_matrix(node));
    //mesh->node_uniform.joint_matrix[3] = mesh->skins[0]->inverse_bind_matrix[3];
}

void Skeletal::load_skin(EMesh* mesh, tinygltf::Model &gltf_model){
    for(tinygltf::Skin &source_skin: gltf_model.skins){
        Skin *new_skin = new Skin{};
        if(source_skin.skeleton > -1){
            new_skin->skeleton_root = node_from_index(mesh, source_skin.skeleton);
        }
        for(int joint_index : source_skin.joints){
            Node* node = node_from_index(mesh, joint_index);
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

        mesh->skins.push_back(new_skin);
    }
    
}

Node* Skeletal::find_node(Node* parent, uint32_t index){
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

Node* Skeletal::node_from_index(EMesh* mesh, uint32_t index){
    Node* node_found = nullptr;
    for(auto &node : mesh->nodes){
        node_found = find_node(node,index);
        if(node_found)
            break;
    }
    return node_found;
}

void NodeManager::update(Node* node){
    if(node->name != "")
        std::cout << "update node: " << node->name << std::endl;
    if(node->mesh){
        glm::mat4 global_transform_mat = get_global_matrix(node);
        EMesh* mesh = node->mesh;
        if(node->skin){
            std::cout << "SKIN update" << std::endl;
            Skin* skin = node->skin;
            mesh->node_uniform.matrix = global_transform_mat;

            glm::mat4 inverse_transform = glm::inverse(global_transform_mat);

            size_t joints_number = skin->joints.size();
            mesh->node_uniform.joint_count = (float)joints_number;

            for(size_t i = 0; i< joints_number ;i++){
                Node* joint_node = skin->joints[i];
                std::cout << "Joint UPDATE: " << joint_node->name << std::endl;                
                
                glm::mat4 joint_mat = inverse_transform * 
                                        get_global_matrix(joint_node);//  * 
                                        skin->inverse_bind_matrix[i];

                mesh->node_uniform.joint_matrix[i] = joint_mat;
            }
        }
    }
    for(auto& child : node->children){
        NodeManager::update(child);
    }
}

void NodeManager::update(EMesh* mesh, Node* node){
    
   
        glm::mat4 global_transform_mat = get_global_matrix(node);
        Skin* skin = mesh->skins[0];

            glm::mat4 inverse_transform = glm::inverse(global_transform_mat);           

            int i = 3;
                Node* joint_node = skin->joints[i];
                std::cout << "Joint UPDATE: " << joint_node->name << std::endl;                
                
                glm::mat4 joint_mat = inverse_transform * 
                                        get_global_matrix(joint_node) * 
                                        skin->inverse_bind_matrix[i];

                mesh->node_uniform.joint_matrix[i] = joint_mat;
            
        
        
}

glm::mat4 NodeManager::get_local_matrix(Node* node){
    return glm::translate(glm::mat4(1.0f),node->Translation) * glm::mat4(node->Rotation) * node->matrix;
}

glm::mat4 NodeManager::get_global_matrix(Node* node){
    glm::mat4 local_matrix = get_local_matrix(node);
    Node* node_parent = node->parent;
    while(node_parent){        
        local_matrix = get_local_matrix(node_parent) * local_matrix;
        node_parent = node_parent->parent;
    }
    return local_matrix;

}

void MeshManager::load_primitives_data(EMesh* mesh, tinygltf::Model &gltf_model){
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
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

        mesh->vertices = vertices;
        mesh->indices = indices;
    }


}