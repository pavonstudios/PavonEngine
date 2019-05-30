#ifndef VERTEX_H
#define VERTEX_H
#include <unordered_map>
#include <vector>
#include <array>

#ifndef ANDROID
    #ifdef VULKAN
        #include <vulkan/vulkan.h>
    #else
       // #include <GL/gl.h>
    #endif
    #include <glm/glm.hpp>
#else
    #include <GLES2/gl2.h>
    #include "glm/glm.hpp"
#endif






#endif
