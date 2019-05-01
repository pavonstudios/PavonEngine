#include "../engine.h"
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
GUI::~GUI(){
    for(auto element : elements){
        delete element;
    }
}
GUI::GUI(Engine* engine){
    this->engine = engine;
    EMesh* triangle = new EMesh();
    
    Vertex vert1{};
    vert1.pos = glm::vec3(-1.0,1.0,0.0);
    vert1.texCoord = vec2(0,1);

    Vertex vert2{};
    vert2.pos = glm::vec3(-1.0,-1.0,0.0);
    vert2.texCoord = vec2(0,0);

    Vertex vert3{};
    vert3.pos = glm::vec3(1.0,1.0,0.0);  
    vert3.texCoord = vec2(1,1);

    Vertex vert4{};
    vert4.pos = glm::vec3(1.0,-1.0,0.0);
    vert4.texCoord = vec2(1,0); 



    if(!engine->loading){
        /* vert1.pos = glm::vec3(0.0,0.0,0.0);

        
        vert2.pos = glm::vec3(0.0,800.0,0.0);

    
        vert3.pos = glm::vec3(800,600,0.0);  

    
        vert4.pos = glm::vec3(600.0,0.0,0.0); */
    }
    

            
    triangle->vertices.push_back(vert1);
    triangle->vertices.push_back(vert2);        
    triangle->vertices.push_back(vert3);
    triangle->vertices.push_back(vert4);
    
    
    triangle->texture_path = "Game/Assets/textures/GUI/white_logo.jpg";
    //triangle->texture_path = "Game/Assets/textures/character.jpg";

    #ifdef VULKAN
        triangle->data.fragment_shader_path = "Game/Assets/shaders/red.spv";
        triangle->data.vertex_shader_path = "Game/Assets/shaders/simple_vert.spv";
        triangle->data.draw_type = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        triangle->data.mesh_type = MESH_TYPE_STATIC;
        //triangle->init(engine->vulkan_device);
        triangle->texture.format = VK_FORMAT_R8G8B8A8_UNORM;	
    #endif
    #ifdef ES2
    if(engine->loading)
        triangle->data.fragment_shader_path = engine->assets.path("frag.glsl");
    else{
        triangle->data.fragment_shader_path = engine->assets.path("shaders/gles/red.glsl");

    }
        
    triangle->data.vertex_shader_path = engine->assets.path("shaders/gles/triangle_vert_shader.glsl");
    #endif

    triangle->bIsGUI = true;
    this->mesh = triangle;
    Button* button = new Button(mesh);
    button->position = vec2(100,100);
    button->size = vec2(30,15);
    button->name = "jump";
    elements.push_back((UIElement*)button);

    #if defined (ES2) || defined (ANDROID) 
    engine->meshes.push_back(this->mesh);
    #endif
}

void GUI::calculate_mouse_position(){
    float x = engine->input.mousex;
    float y = engine->input.mousey;
    //std::cout << x << " " << y << std::endl;
    Button* button = static_cast<Button*>(elements[0]);
    float minx = button->position.x - button->size.x;
    float maxx = button->position.x + button->size.x;
    float miny = button->position.y - button->size.y;
    float maxy = button->position.y + button->size.y;

    button->pressed = false;
    if(engine->input.left_button_pressed){ 
        if(minx <= x && maxy >= y){
            if(miny <= y && maxx >= x)
                button->pressed = true;
        }else{
            button->pressed = false;
        }
    }
    
}

void GUI::update_elements_mvp(){
    glm::mat4 mat = glm::mat4(1.0);
    for(auto element : elements){
        mat4 projection = glm::ortho(   0.0f, 1.0f*engine->window_manager.window_width, 
                                        1.0f*engine->window_manager.window_height, 0.0f);
                                        
        mat4 image_scale = glm::scale(mat,vec3(element->size.x,element->size.y,0));
        mat4 model_mat = translate(mat,vec3(element->position.x,element->position.y,0));
        model_mat = model_mat * image_scale;
        mat = projection * model_mat;
        element->mesh->MVP = mat;
    }
}

bool GUI::is_button_pressed(std::string name){
    UIElement* ui;
    for(auto element : elements){
        if(element->name == name){
            ui = element;
            break;
        }
    }
    if(ui){
        Button* button = static_cast<Button*>(ui);
        if(button->pressed){
            return true;
        }
    }
    return false;
}