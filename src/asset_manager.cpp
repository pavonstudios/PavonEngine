#include "asset_manager.h"
#include "iostream"

AssetManager::AssetManager() {
	this->asset_path = "Game/Assets/";
}

void AssetManager::free_image(void * pixels){
    stbi_image_free(pixels);
}

std::string AssetManager::convert_to_asset_folder_path(std::string path_to_convert){
    return asset_path + path_to_convert;
}
std::string AssetManager::path(std::string path_to_convert){
    #ifdef ANDROID
    asset_path = "";
    #endif   
    #ifdef WINDOWS
		this->asset_path = "Game/Assets/";
        for(char& c : this->asset_path){
            if(c == '/'){
                c = '\\';
            }
        }
        for(char& c : path_to_convert){
            if(c == '/'){
                c = '\\';
            }
        }    
        
    #endif
    if(asset_path == ""){
        return path_to_convert;
    }
    return asset_path + path_to_convert;
}

Image AssetManager::load_and_get_size(std::string texture_path){
    int texWidth, texHeight, texChannels;
    unsigned char* data = nullptr;
    stbi_uc* pixels = nullptr;

    #ifndef VULKAN
        data = stbi_load(texture_path.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb);
        if (!data) {
		std::string error = "failed to load texture image! " + texture_path;
		throw std::runtime_error(error);
	    }
    #endif // DEBUG  

	
    #if defined (VULKAN) || defined (DX11)
         pixels = stbi_load(texture_path.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
           
        if (!pixels) {
                std::string error = "failed to load texture image! " + texture_path;
                throw std::runtime_error(error);
        }
    #endif // DEBUG
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
