#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H
#include "string"

#ifdef ANDROID
#include <android/asset_manager.h>
#include <GLES2/gl2.h>
#endif
#include "stb_image.h"

#include "texture.h"

class AssetManager{
public:
    std::string AssetPath = "Game/Assets/";
    std::string convert_to_asset_folder_path(std::string path_to_convert);
    void free_image(void * pixels);
    Image load_and_get_size(std::string texture_path);
	//int load_model_gltf(const char* path);
    #ifdef ANDROID
    Image load_bmp(const char* filename, AAssetManager * assetManager);
    #endif
    std::string path(std::string path_to_convert);
	
};

#endif