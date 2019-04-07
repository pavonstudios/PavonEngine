#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H
#include "string"

struct image_size{
    int width;
    int heigth;
    void * pPixels;
};

/*
Load textures
*/
class AssetManager{
public:
    void free_image(void * pixels);
    image_size load_and_get_size(std::string texture_path);
	int load_model_gltf(const char* path);
	
};
#endif