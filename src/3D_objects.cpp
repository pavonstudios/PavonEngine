


#include "3D_objects.h"
#include "iostream"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#ifdef GLTF
using namespace engine;
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

void EMesh::load_node(engine::Node *parent, const tinygltf::Node &gltf_node){
    Node *new_node = new Node{};
    new_node->parent = parent;
    new_node->matrix = glm::mat4(1.0f);
    
    //some nodes do not contain transform information
    if(gltf_node.translation.size() == 3)
        new_node->Translation = glm::make_vec3(gltf_node.translation.data());

    if(gltf_node.rotation.size() == 4)
        new_node->Rotation = glm::make_quat(gltf_node.rotation.data());

    if(gltf_node.matrix.size() == 16)
        new_node->matrix = glm::make_mat4x4(gltf_node.matrix.data());

    if(gltf_node.children.size() > 0){
        for(size_t i = 0;i < gltf_node.children.size();i++){
            load_node(new_node,gltf_model.nodes[gltf_node.children[i]]);
        }
    }

    if(!parent){
        nodes.push_back(new_node);
    }else{
        parent->children.push_back(new_node);
    }
    linear_nodes.push_back(new_node);
}
int EMesh::load_model_gltf(const char* path){
    
    
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    bool ret = loader.LoadASCIIFromFile(&gltf_model, &err, &warn, path);
    //bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, argv[1]); // for binary glTF(.glb)
    
    for(size_t i = 0; i < gltf_model.nodes.size();i++){
        load_node(nullptr,gltf_model.nodes[i]);
    }

    for(auto primitive : gltf_model.meshes[0].primitives){
        uint32_t indexStart = static_cast<uint32_t>(indices.size());
        uint32_t vertexStart = static_cast<uint32_t>(vertices.size());
        uint32_t indexCount = 0;
        uint32_t vertexCount = 0;
        const float *bufferPos = nullptr;
        const uint16_t *bufferJoints = nullptr;
		const float *bufferWeights = nullptr;

        const tinygltf::Accessor &posAccessor = gltf_model.accessors[primitive.attributes.find("POSITION")->second];
        const tinygltf::BufferView &posView = gltf_model.bufferViews[posAccessor.bufferView];
        bufferPos = reinterpret_cast<const float *>(&(gltf_model.buffers[posView.buffer].data[posAccessor.byteOffset + posView.byteOffset]));
        vertexCount = static_cast<uint32_t>(posAccessor.count);

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
                return 2;
            }
    }//end loop primitives

    load_skins();
    for(auto node : linear_nodes){
        if(node->skin_index > -1)
            node->skin = skins[node->skin_index];
        
    }   

    return 1;
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
#else
EMesh::EMesh(){
    
}
#endif
EMesh::~EMesh(){
    if(graphics_pipeline != VK_NULL_HANDLE)
        vkDestroyPipeline(vulkan_device->logicalDevice,graphics_pipeline,nullptr);
    for(auto buffer: uniformBuffers){
        vkDestroyBuffer(vulkan_device->logicalDevice,buffer,nullptr);
    }
    for(auto buffer: uniform_node_buffers){
        vkDestroyBuffer(vulkan_device->logicalDevice,buffer,nullptr);
    }
    for(auto uniform_memory : uniformBuffersMemory){
        vkFreeMemory(vulkan_device->logicalDevice,uniform_memory, nullptr);
    }
    for(auto uniform_memory : uniform_node_buffer_memory){
        vkFreeMemory(vulkan_device->logicalDevice,uniform_memory, nullptr);
    }
    vkDestroyBuffer(vulkan_device->logicalDevice,indexBuffer,nullptr);
    vkDestroyBuffer(vulkan_device->logicalDevice,vertices_buffer,nullptr);
    
}

bool EMesh::load_model2(const char * path){

            std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
            std::vector< glm::vec3 > temp_vertices;
            std::vector< glm::vec2 > temp_uvs;
            std::vector< glm::vec3 > temp_normals;

            FILE * file = fopen(path, "r");
            if( file == NULL ){
                printf("Impossible to open the file !\n");
                return false;
            }
            while( 1 ){

                    char lineHeader[128];
                    // read the first word of the line
                    int res = fscanf(file, "%s", lineHeader);
                    if (res == EOF)
                        break; // EOF = End Of File. Quit the loop.

                    // else : parse lineHeader
                    if ( strcmp( lineHeader, "v" ) == 0 ){
                        glm::vec3 vertex;
                        fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
                        temp_vertices.push_back(vertex);
                    }else if ( strcmp( lineHeader, "f" ) == 0 ){
                        std::string vertex1, vertex2, vertex3;
                        unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
                        int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
                        if (matches != 9){
                            printf("File can't be read by our simple parser : ( Try exporting with other options\n");
                            return false;
                        }
                        vertexIndices.push_back(vertexIndex[0]);
                        vertexIndices.push_back(vertexIndex[1]);
                        vertexIndices.push_back(vertexIndex[2]);
                        uvIndices    .push_back(uvIndex[0]);
                        uvIndices    .push_back(uvIndex[1]);
                        uvIndices    .push_back(uvIndex[2]);
                        normalIndices.push_back(normalIndex[0]);
                        normalIndices.push_back(normalIndex[1]);
                        normalIndices.push_back(normalIndex[2]);
                    }

                    for( unsigned int i=0; i<vertexIndices.size(); i++ ){
                        unsigned int vertexIndex = vertexIndices[i];
                        glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
                        simple_vertices.push_back(vertex);
                    }
                    
            }
            return true;
        }
void EMesh::load_model(std::string model_path){
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, model_path.c_str())) {
            throw std::runtime_error(warn + err);
        }

        std::unordered_map<Vertex, uint32_t> uniqueVertices = {};

        for (const auto& shape : shapes) {
            for (const auto& index : shape.mesh.indices) {
                Vertex vertex = {};

                vertex.pos = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                };

                vertex.texCoord = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                };

                vertex.color = {1.0f, 1.0f, 1.0f};

                if (uniqueVertices.count(vertex) == 0) {
                    uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex);
                }

                indices.push_back(uniqueVertices[vertex]);
            }
        }
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


