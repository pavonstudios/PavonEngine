#include "asset_manager.h"
#include "iostream"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//#include "tiny_class.h"
void AssetManager::free_image(void * pixels){
    stbi_image_free(pixels);
}
image_size AssetManager::load_and_get_size(std::string texture_path){
    int texWidth, texHeight, texChannels;
    stbi_uc* pixels = stbi_load(texture_path.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb);
        
    unsigned char* data = stbi_load(texture_path.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb);
    if (!pixels) {
            std::string error = "failed to load texture image! " + texture_path;
            throw std::runtime_error(error);
    }
    image_size size = {texWidth, texHeight, pixels, data};
    return size;
}

#ifdef ANDROID
image_size AssetManager::load_bmp( const char * filename ,AAssetManager * assetManager)
{

    AAsset* file = AAssetManager_open(assetManager,filename, AASSET_MODE_BUFFER);

    size_t size = AAsset_getLength(file);
    width = 1024;
    height = 1024;

    unsigned char* content = new unsigned char[size];

    AAsset_read(file, content,size);

    //free( data );

    image_size image_Data = {width, height, content, content};
    return image_Data;
}

#endif
/* int AssetManager::load_model_gltf(const char* path){    
    tinygltf::Model gltf_model;
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    bool ret = loader.LoadASCIIFromFile(&gltf_model, &err, &warn, path);
    //bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, argv[1]); // for binary glTF(.glb)
    
   
    return 1;
} */