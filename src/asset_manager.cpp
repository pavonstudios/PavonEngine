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
    stbi_uc* pixels = stbi_load(texture_path.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        
    
    if (!pixels) {
            std::string error = "failed to load texture image! " + texture_path;
            throw std::runtime_error(error);
    }
    image_size size = {texWidth, texHeight, pixels};
    return size;
}
#ifdef ANDROID
GLuint AssetManager::load_bmp( const char * filename ,AAssetManager * assetManager)
{

    GLuint texture;

    int width, height;



    AAsset* file = AAssetManager_open(assetManager,filename, AASSET_MODE_BUFFER);

    size_t size = AAsset_getLength(file);
    size_t  file_length = width * height * 3;
    unsigned char* data = (unsigned char *)malloc( width * height * 3 );

    char* content = new char[size*3];

    AAsset_read(file, content,size);

    if ( file == NULL ) return 0;
    width = 1024;
    height = 1024;
    //data = (unsigned char *)malloc( width * height * 3 );
    //int size = fseek(file,);
    //fread( data, width * height * 3, 1, file );
    //fclose( file );

    for(int i = 0; i < width * height ; ++i)
    {
        int index = i*3;
        unsigned char B,R;
        B = content[index];
        R = content[index+2];

        content[index] = R;
        content[index+2] = B;

    }


    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );
    //glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST );


    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT );
    //gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height,GL_RGB, GL_UNSIGNED_BYTE, data );
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,1024, 1024,0,GL_RGB, GL_UNSIGNED_BYTE, data);

    free( data );

    return texture;
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