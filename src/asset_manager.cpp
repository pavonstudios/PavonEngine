#include "asset_manager.h"
#include "iostream"



void AssetManager::free_image(void * pixels){
    stbi_image_free(pixels);
}

std::string AssetManager::convert_to_asset_folder_path(std::string path_to_convert){
    return AssetPath + path_to_convert;
}
std::string AssetManager::path(std::string path_to_convert){
    #ifdef ANDROID
    AssetPath = "";
    #endif   
    return AssetPath + path_to_convert;
}

Image AssetManager::load_and_get_size(std::string texture_path){
    int texWidth, texHeight, texChannels;
    stbi_uc* pixels = stbi_load(texture_path.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        
    unsigned char* data = stbi_load(texture_path.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb);
   
    if (!pixels) {
            std::string error = "failed to load texture image! " + texture_path;
            throw std::runtime_error(error);
    }
    Image size = {texWidth, texHeight, pixels, data};
    return size;
}

#ifdef ANDROID
Image AssetManager::load_bmp( const char * filename ,AAssetManager * assetManager)
{

    AAsset* file = AAssetManager_open(assetManager,filename, AASSET_MODE_BUFFER);

    size_t size = AAsset_getLength(file);
    int width = 1024;
    int height = 1024;

    unsigned char* content = new unsigned char[size];

    AAsset_read(file, content,size);

     int w, h, comp, req_comp;
    req_comp = 3;

    unsigned char* decoded = stbi_load_from_memory(content, (int)size, &w, &h, &comp, req_comp);
    //free( data );

    Image image_Data = {width, height, decoded, decoded};
    return image_Data;
}

#endif
