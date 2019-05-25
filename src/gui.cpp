#include "engine.h"
#include "gui.hpp"
#ifndef ANDROID
    #include <glm/glm.hpp>
    #include <glm/gtc/matrix_transform.hpp>
#else
    #include "glm/glm.hpp"
    #include "glm/gtc/matrix_transform.hpp"
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
   
    triangle->vertices.clear();
    triangle->vertices.push_back(vert1);
    triangle->vertices.push_back(vert2);        
    triangle->vertices.push_back(vert3);
    triangle->vertices.push_back(vert4);
    
    
    triangle->texture_path = engine->assets.path("textures/GUI/white_logo.jpg");

    #ifdef VULKAN
        triangle->data_shader.fragment_shader_path = "Game/Assets/shaders/vk/red.spv";
        triangle->data_shader.vertex_shader_path = "Game/Assets/shaders/vk/simple_vert.spv";
        triangle->data_shader.draw_type = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        triangle->data_shader.mesh_type = MESH_TYPE_STATIC;
        //triangle->init(engine->vulkan_device);
        triangle->texture.format = VK_FORMAT_R8G8B8A8_UNORM;	
    #endif
    #ifdef ES2
    if(engine->loading){
        triangle->data_shader.fragment_shader_path = engine->assets.path("shaders/gles/frag.glsl");
        engine->meshes.clear();
    }
    else{
        triangle->data_shader.fragment_shader_path = engine->assets.path("shaders/gles/red.glsl");

    }
        
    triangle->data_shader.vertex_shader_path = engine->assets.path("shaders/gles/vert_mvp.glsl");
    #endif
    #ifdef ANDROID
    triangle->data_shader.fragment_shader_path = engine->assets.path("shaders/gles/frag2.glsl");
    //data.fragment_shader_path = assets.path("shaders/gles/frag_uv_color.glsl");


    triangle->data_shader.vertex_shader_path = engine->assets.path("shaders/gles/vert_mvp.glsl");
    #endif
    triangle->type = -1;
    triangle->bIsGUI = true;
    triangle->name = "gui button";
    Button* button = new Button(triangle);
    button->relative_position = vec2(250,250);
    button->relative_to = POSITION_RELATIVE_LEFT_BOTTON;
    button->size = vec2(30,30);
    button->name = "jump";
    elements.push_back((UIElement*)button);

    #if defined (ES2) || defined (ANDROID) 
        this->mesh_elements.push_back(triangle);
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
        engine->input.move_camera = true;
        engine->input.mouse_movement(engine,x,y);
        if(minx <= x && maxy >= y){
            if(miny <= y && maxx >= x){
                button->pressed = true;
                if(!button->move)
                    button->saved_position = button->position;
                button->move = true;
            }
                
        }else{
            button->pressed = false;
        }
    }else{
        if(!engine->edit_mode){
            #if defined (ANDROID)
                engine->input.move_camera = false;
            #endif
        }
       
    }
#ifdef ANDROID
    if(minx <= x && maxy >= y){
        if(miny <= y && maxx >= x)
            button->pressed = true;
    }else{
        button->pressed = false;
    }
#endif
        
     if(engine->input.left_button_pressed){ 
        if(button->move){
            button->position.x = x;
            button->position.y = y;
            this->update_elemete_position((UIElement*)button);

            float diff_x = button->saved_position.x - x;
            float diff_y = button->saved_position.y - y;

            
            if(diff_y < 0){
                engine->input.S.bIsPressed = true;
            }
            if(diff_y > 0){
                 engine->input.W.bIsPressed = true;
            }
            if(diff_x < 0){
                engine->input.D.bIsPressed = true;
            }
            if(diff_x > 0){
                 engine->input.A.bIsPressed = true;
            }

        }
     }

    if(engine->input.left_button_release){
        if(button->move){
            engine->input.left_button_release = false;
            button->move = false;
            button->position = button->saved_position;
            this->update_elemete_position((UIElement*)button);
            engine->input.S.bIsPressed = false;
            engine->input.W.bIsPressed = false;
            engine->input.D.bIsPressed = false;
            engine->input.A.bIsPressed = false;
        }
        
    }



}
void GUI::update_elemete_position(UIElement* element){
     glm::mat4 mat = glm::mat4(1.0);
     mat4 projection = glm::ortho(   0.0f, 1.0f*engine->window_manager.window_width, 
                                        1.0f*engine->window_manager.window_height, 0.0f);
                                        
    mat4 image_scale = glm::scale(mat,vec3(element->size.x,element->size.y,0));
    mat4 model_mat = translate(mat,vec3(element->position.x,element->position.y,0));

    model_mat = model_mat * image_scale;
    mat = projection * model_mat;
    element->mesh->MVP = mat;
}
void GUI::update_engine_meshes(){
    #ifdef ES2
    engine->meshes.push_back(mesh_elements[0]);
    engine->linear_meshes.push_back(mesh_elements[0]);
    #endif
}
void GUI::update_elements_mvp(){
    glm::mat4 mat = glm::mat4(1.0);
    for(auto element : elements){
        mat4 projection = glm::ortho(   0.0f, 1.0f*engine->window_manager.window_width, 
                                        1.0f*engine->window_manager.window_height, 0.0f);
                                        
        mat4 image_scale = glm::scale(mat,vec3(element->size.x,element->size.y,0));
        mat4 model_mat = translate(mat,vec3(element->position.x,element->position.y,0));
        if(element->relative_to == POSITION_RELATIVE_LEFT_BOTTON){
            float x = element->relative_position.x;
            float y = engine->window_manager.window_height - element->relative_position.y;
            element->position.x = x;
            element->position.y = y;
            model_mat = translate(mat,vec3(x,y,0));
        }
       
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