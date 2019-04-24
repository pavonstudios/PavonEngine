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
GUI::GUI(Engine* engine){
	this->engine = engine;
		EMesh* triangle = new EMesh();
        /* glm::vec2 texcoor = glm::vec2(0.f);
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

		triangle->indices.push_back(1); */
		triangle->load_model_gltf("Game/Assets/models/button01.gltf");

		
		triangle->texture_path = "Game/Assets/textures/car01.jpg";
		#ifdef VULKAN
            triangle->data.fragment_shader_path = "Game/Assets/shaders/red.spv";
            triangle->data.vertex_shader_path = "Game/Assets/shaders/simple_vert.spv";
            triangle->data.draw_type = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
            triangle->data.mesh_type = MESH_TYPE_STATIC;

            triangle->init(engine->vulkan_device);
            triangle->texture.format = VK_FORMAT_R8G8B8A8_UNORM;	
		#endif
        #ifdef ES2
            triangle->data.fragment_shader_path = "Game/Assets/frag.glsl";
            triangle->data.vertex_shader_path = "Game/Assets/vert.glsl";
        #endif
		
		this->mesh = triangle;

		engine->meshes.push_back(this->mesh);
}