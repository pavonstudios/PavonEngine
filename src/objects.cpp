
#include "engine.h"
#include "objects.h"
#include "iostream"

#include <thread>

#if defined(ES2) && defined (LINUX)
    #include <GLES2/gl2.h>
#endif
#ifdef GL
    #include <GL/gl.h>
    #include <GL/glext.h>
#endif

#ifdef GLTF
using namespace engine;
EMesh::EMesh(){
    this->vertices.clear();
}


#ifdef VULKAN
EMesh::EMesh(vks::VulkanDevice* vulkan_device){
    this->vulkan_device = vulkan_device;    
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
void MeshManager::create_buffers(Engine* engine, const std::vector<EMesh*>& meshes){
    for(EMesh* mesh : meshes){   
        #ifdef VULKAN
            if(mesh->type == MESH_TYPE_SKINNED){
                    //node uniform buffer
                mesh->node_uniform.matrix = glm::mat4(1.0);
                    

                VkDeviceSize bufferSize = sizeof(NodeUniform);

                mesh->uniform_node_buffers.resize(3);
                mesh->uniform_node_buffer_memory.resize(3);

                for (size_t i = 0; i < 3; i++) {
                    vulkan_device->createBuffer(
                        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                        sizeof(NodeUniform),
                        &mesh->uniform_node_buffers[i],
                        &mesh->uniform_node_buffer_memory[i],
                        &mesh->node_uniform);               
                }
            }
           
        #endif        
    }
    
}
void MeshManager::create_buffers(EMesh* mesh){

    #ifdef VULKAN
    //node uniform buffer
    mesh->node_uniform.matrix = glm::mat4(1.0);
        

    VkDeviceSize bufferSize = sizeof(NodeUniform);

    mesh->uniform_node_buffers.resize(3);
    mesh->uniform_node_buffer_memory.resize(3);

    for (size_t i = 0; i < 3; i++) {
        vulkan_device->createBuffer(
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            sizeof(NodeUniform),
            &mesh->uniform_node_buffers[i],
            &mesh->uniform_node_buffer_memory[i],
            &mesh->node_uniform);               
    }
    #endif
}

#ifdef LINUX




void MeshManager::load_textures_gltf(EMesh* mesh, tinygltf::Model & gltf_model){
    EImageData image_data_struct = {};
    if(gltf_model.images.size() > 0){
        tinygltf::Image tiny_image = gltf_model.images[0];

        unsigned char * image_data = nullptr;
        uint32_t image_size = 0;

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


        mesh->texture = image_data_struct;
    }
    
    
}
#endif // LINUX
#ifdef ANDROID
int MeshManager::load_mode_gltf_android(EMesh* mesh, const char* path, AAssetManager* assetManager){
        
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    bool ret = loader.LoadASCIIFromFileAndroid(&mesh->gltf_model, &err, &warn, path,0,assetManager);
    //bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, argv[1]); // for binary glTF(.glb)
    if(ret){
        load_primitives_data(mesh, mesh->gltf_model);
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
#ifdef VULKAN
void MeshManager::clean_pipeline_from_meshes(std::vector<EMesh*> meshes){
    for(EMesh* mesh : meshes){
        vkDestroyPipeline(vulkan_device->logicalDevice,mesh->graphics_pipeline,nullptr);  

    }
}
#endif


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
    //load_textures_gltf(mesh, mesh->gltf_model);    

    
    return 1;
}




void MeshManager::load_primitives_data(EMesh* mesh, tinygltf::Model &gltf_model){
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    for(size_t i = 0; i < gltf_model.meshes.size();i++){
        for(auto primitive : gltf_model.meshes[i].primitives){
            //uint32_t indexStart = static_cast<uint32_t>(indices.size());
            uint32_t vertexStart = static_cast<uint32_t>(vertices.size());
            //uint32_t indexCount = 0;
            //uint32_t vertexCount = 0;
            const float *bufferPos = nullptr;
            const uint16_t *bufferJoints = nullptr;
            const float *bufferWeights = nullptr;
            const float *bufferTexCoordSet0 = nullptr;

            const tinygltf::Accessor &posAccessor = gltf_model.accessors[primitive.attributes.find("POSITION")->second];
            const tinygltf::BufferView &posView = gltf_model.bufferViews[posAccessor.bufferView];
            bufferPos = reinterpret_cast<const float *>(&(gltf_model.buffers[posView.buffer].data[posAccessor.byteOffset + posView.byteOffset]));
           // vertexCount = static_cast<uint32_t>(posAccessor.count);

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

            //indexCount = static_cast<uint32_t>(accessor.count);
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
        mesh->name = gltf_model.meshes[0].name;
    }


}

void Objects::update_positions(Engine* engine, const TranslationUpdate &translation){
    for(size_t i = 0; i < translation.movements.size(); i++){
        glm::vec3 movement =  translation.movements[i].direction * translation.movements[i].value * engine->deltaTime;
        translation.meshes[i]->model_matrix = glm::translate(translation.meshes[i]->model_matrix,movement);		
        translation.meshes[i]->location_vector = translation.meshes[i]->location_vector + movement;
    }
}

void Objects::translate(Engine* engine, EMesh* mesh, Movement& movement){
    engine->tranlation_update.meshes.push_back(mesh);
    engine->tranlation_update.movements.push_back(movement);
}

EMesh* MeshManager::mesh_by_name(std::string name){
    for(EMesh* mesh : engine->linear_meshes){
        if(mesh->name == name){
            return mesh;
        }
    }
    std::cout << "mesh not found: " << name << std::endl;
    return nullptr;
}

void TexturesManager::load_textures_to_cpu_memory(const std::vector<EMesh*> meshes){
    int core_count = std::thread::hardware_concurrency();
    int meshes_count = meshes.size();

    div_t division = std::div(meshes_count,core_count);
    
    std::vector< std::vector<EMesh*> > parts;

    int cout_to_div = division.quot;
    int counter = 0;
     std::vector<EMesh*> part;
    for(int i = 0 ; i < meshes.size() - 1 ; i++){
       
        part.push_back(meshes[i]);
        counter++;
        if(counter == cout_to_div){
            
            parts.push_back(part);
            counter = 0;
            part.clear();
        }        
    }
    if(division.rem >= 1){
        std::vector<EMesh*> part;
        part.push_back(meshes.back());
        parts.push_back(part);
    }

    std::vector<std::thread> threads;
    for(int t = 0 ; t < parts.size() ; t++){
        std::thread load (TexturesManager::load_texture,this->engine, std::ref(parts[t]) );
        threads.push_back(std::move(load));
    }
    for(int v = 0 ; v < threads.size() ; v++){
        threads[v].join();
    }

    
    
}


void TexturesManager::load_texture(Engine * engine, std::vector<EMesh*> &meshes){
    for(EMesh* mesh : meshes){
         if (mesh->texture.hasTexture)
        {
        
        }
        else
        {
            if (mesh->texture_path != ""){
                Image image;
                image = engine->assets.load_and_get_size(mesh->texture_path.c_str());
                mesh->image = image;
            }
        }
    }
   
}

void engine::TexturesManager::free_textures_from_cpu_memory(Engine* engine, const std::vector<EMesh*>& meshes)
{
	for (EMesh* mesh : meshes) {
		engine->assets.free_image(mesh->image.pPixels);
		engine->assets.free_image(mesh->image.data);
	}
}
