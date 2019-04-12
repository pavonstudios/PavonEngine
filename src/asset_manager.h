#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H
#include "string"

#ifdef ANDROID
#include <android/asset_manager.h>
#include <GLES2/gl2.h>
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
	//int load_model_gltf(const char* path);
    #ifdef ANDROID
    GLuint load_bmp(const char* filename, AAssetManager * assetManager);
    #endif
	
};

#endif