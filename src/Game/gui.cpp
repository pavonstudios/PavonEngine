#include "gui.hpp"
#ifndef ANDROID
    #include <glm/glm.hpp>
    #include <glm/gtc/matrix_transform.hpp>
#else
    #include "../glm/glm.hpp"
    #include "../glm/gtc/matrix_transform.hpp"
#endif
using namespace glm;
using namespace engine;
GUI::GUI(){
	EMesh* triangle = new EMesh();
        glm::vec2 texcoor = glm::vec2(0.f);
        glm::vec4 posss = glm::vec4(0.f);
        glm::vec4 witjj = glm::vec4(0.f);
        Vertex vert1{};
        vert1.pos = glm::vec3(1.0,1.0,0.0);
        Vertex vert2{};
        vert2.pos = glm::vec3(0.0,0.0,0.0);
        Vertex vert3{};
        vert3.pos = glm::vec3(0.0,1.0,0.0);

        vert1.texCoord = glm::vec2(1.0f,1.0f);
        vert1.color = vec3(1.0,0.0,0.0);
        vert1.joint0 = glm::vec4(0.0f);
        vert1.weight0 = glm::vec4(0.0f);

           vert3.texCoord = glm::vec2(0.0f,0.0f);
        vert3.joint0 = glm::vec4(0.0f);
         vert3.color = vec3(0.0,0.0,1.0);
        vert3.weight0 = glm::vec4(0.0f);

           vert2.texCoord = glm::vec2(1.0f,0.0f);
        vert2.joint0 = glm::vec4(0.0f);
         vert2.color = vec3(0.0,1.0,0.0);
        vert2.weight0 = glm::vec4(0.0f);

        triangle->vertices.push_back(vert1);
        triangle->vertices.push_back(vert2);
        triangle->vertices.push_back(vert3);
}