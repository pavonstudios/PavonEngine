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
    
    Vertex vert1{};
    vert1.pos = glm::vec3(-1.0,1.0,0.0);

    Vertex vert2{};
    vert2.pos = glm::vec3(-1.0,-1.0,0.0);

    Vertex vert3{};
    vert3.pos = glm::vec3(1.0,1.0,0.0);  

    Vertex vert4{};
    vert4.pos = glm::vec3(1.0,-1.0,0.0); 



/* 
    vert1.pos = glm::vec3(0.0,100.0,0.0);

    
    vert2.pos = glm::vec3(0.0,-200.0,0.0);

   
    vert3.pos = glm::vec3(1000.0,1000.0,0.0);  

   
    vert4.pos = glm::vec3(1000.0,0.0,0.0); */

            
    triangle->vertices.push_back(vert1);
    triangle->vertices.push_back(vert2);        
    triangle->vertices.push_back(vert3); 
    triangle->vertices.push_back(vert4);
    
    
    triangle->texture_path = "Game/Assets/textures/car01.jpg";
    #ifdef VULKAN
        triangle->data.fragment_shader_path = "Game/Assets/shaders/red.spv";
        triangle->data.vertex_shader_path = "Game/Assets/shaders/simple_vert.spv";
        triangle->data.draw_type = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        triangle->data.mesh_type = MESH_TYPE_STATIC;
        //triangle->init(engine->vulkan_device);
        triangle->texture.format = VK_FORMAT_R8G8B8A8_UNORM;	
    #endif
    #ifdef ES2
        triangle->data.fragment_shader_path = "Game/Assets/shaders/gles/red.glsl";
        triangle->data.vertex_shader_path = "Game/Assets/shaders/gles/triangle_vert_shader.glsl";
    #endif
    triangle->bIsGUI = true;
    this->mesh = triangle;

    #ifdef ES2
    engine->meshes.push_back(this->mesh);
    #endif
}

void GUI::calculate_mouse_position(){
    float x = engine->window_manager.window_width / engine->input.mousex;
    float y = engine->window_manager.window_height / engine->input.mousey;
    x = 1 - x;
    std::cout << x << " " << y << std::endl;

}