
#ifdef GLTF
    #define TINYGLTF_IMPLEMENTATION
    //#define STB_IMAGE_WRITE_IMPLEMENTATION
    #define TINYGLTF_NO_STB_IMAGE
    #define TINYGLTF_NO_STB_IMAGE_WRITE
    #define TINYGLTF_NO_EXTERNAL_IMAGE
    #include "tiny_gltf.h"
#endif

#include "tiny_gltf_classes.h"