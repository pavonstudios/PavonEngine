#include "asset_manager.h"
#include "iostream"
#include "tiny_class.h"
void AssetManager::free_image(void * pixels){
    stbi_image_free(pixels);
}
image_size AssetManager::load_and_get_size(std::string texture_path){
    int texWidth, texHeight, texChannels;
    stbi_uc* pixels = stbi_load(texture_path.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        
    
    if (!pixels) {
            throw std::runtime_error("failed to load texture image!");
    }
    image_size size = {texWidth, texHeight, pixels};
    return size;
}
int AssetManager::load_model_gltf(const char* path){    
    tinygltf::Model gltf_model;
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    bool ret = loader.LoadASCIIFromFile(&gltf_model, &err, &warn, path);
    //bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, argv[1]); // for binary glTF(.glb)
    
   
    return 1;
}