#include "objects.h"
using namespace engine;
void Skeletal::load_data(EMesh* mesh){
    int node_count = mesh->gltf_model.nodes.size();
    for(size_t i = 0; i < node_count;i++){
        mesh->load_node(nullptr,i,mesh->gltf_model.nodes[i]);
    }

    Skeletal::load_skin(mesh, mesh->gltf_model);
    bool isUpdated = false;
    Node* node_with_mesh;
    for(auto node : mesh->linear_nodes){
        if(node->mesh){
            if(mesh->skins.size()>0){
                node->skin = mesh->skins[0];

                if(!isUpdated){
                    NodeManager::update(node);
               // isUpdated = true;
                }
            //for some reason this not work, produce issues in vertices transformation
                node_with_mesh = node;
            }
                
        }
    }     
    //mesh->model_matrix = glm::rotate(mesh->model_matrix,glm::radians(90.0f),glm::vec3(1,0,0)); 

    //update_joints_matrix(mesh, node_with_mesh);

    for(Node* node : mesh->linear_nodes){
        if(node->name == "lower_arm"){
            //NodeManager::update(mesh, node);
        }
    }
}

void NodeManager::update(Node* node){
    if(node->name != "")
        std::cout << "update node: " << node->name << std::endl;
    if(node->mesh){
        glm::mat4 global_transform_mat = get_global_matrix(node);
        EMesh* mesh = node->mesh;
        if(node->skin){
            std::cout << "SKIN update" << std::endl;
            Skin* skin = node->skin;
            mesh->node_uniform.matrix = global_transform_mat;

            glm::mat4 inverse_transform = glm::inverse(global_transform_mat);

            size_t joints_number = skin->joints.size();
            mesh->node_uniform.joint_count = (float)joints_number;

            for(size_t i = 0; i< joints_number ;i++){
                Node* joint_node = skin->joints[i];
                std::cout << "Joint UPDATE: " << joint_node->name << std::endl;                
                
                glm::mat4 joint_mat = inverse_transform * 
                                        get_global_matrix(joint_node);//  * 
                                        skin->inverse_bind_matrix[i];

                mesh->node_uniform.joint_matrix[i] = joint_mat;
            }
        }
    }
    for(auto& child : node->children){
        NodeManager::update(child);
    }
}

void NodeManager::update(EMesh* mesh, Node* node){
    
   
        glm::mat4 global_transform_mat = get_global_matrix(node);
        Skin* skin = mesh->skins[0];

            glm::mat4 inverse_transform = glm::inverse(global_transform_mat);           

            int i = 3;
                Node* joint_node = skin->joints[i];
                std::cout << "Joint UPDATE: " << joint_node->name << std::endl;                
                
                glm::mat4 joint_mat = inverse_transform * 
                                        get_global_matrix(joint_node) * 
                                        skin->inverse_bind_matrix[i];

                mesh->node_uniform.joint_matrix[i] = joint_mat;
            
        
        
}

glm::mat4 NodeManager::get_local_matrix(Node* node){
    glm::mat4 local = glm::translate(glm::mat4(1.0f),node->Translation) * glm::mat4(node->Rotation) * node->matrix;
    glm::mat4 reseted_local_position = glm::translate(glm::mat4(1.0),glm::vec3(0,0,0));
    return reseted_local_position;
}

glm::mat4 NodeManager::get_global_matrix(Node* node){
    glm::mat4 local_matrix = get_local_matrix(node);
    Node* node_parent = node->parent;
    while(node_parent){        
        local_matrix = get_local_matrix(node_parent) * local_matrix;
        node_parent = node_parent->parent;
    }
    return local_matrix;

}

void Skeletal::update_joints_matrix(EMesh* mesh, Node* node){
    mesh->node_uniform.matrix = glm::mat4(1.0);
    mesh->node_uniform.joint_count = 4;
    mesh->node_uniform.joint_matrix[0] = glm::translate(glm::mat4(1.0),glm::vec3(0,0,2));
    mesh->node_uniform.joint_matrix[1] = glm::translate(glm::mat4(1.0),glm::vec3(0,0,0));
    mesh->node_uniform.joint_matrix[2] = glm::translate(glm::mat4(1.0),glm::vec3(0,0,0));
    mesh->node_uniform.joint_matrix[3] = glm::translate(glm::mat4(1.0),glm::vec3(0,0,0));

    glm::mat4 rot = glm::rotate(glm::mat4(1.0),glm::radians(-45.f),glm::vec3(0,1,0));
    glm::mat4 rot2 = glm::rotate(glm::mat4(1.0),glm::radians(45.f),glm::vec3(0,1,0));
    //mesh->node_uniform.joint_matrix[2] = mesh->node_uniform.joint_matrix[2] * rot;

    mesh->node_uniform.joint_matrix[1] = mesh->node_uniform.joint_matrix[0] * mesh->node_uniform.joint_matrix[1];
     //mesh->node_uniform.joint_matrix[2] = mesh->node_uniform.joint_matrix[2] * rot;
     mesh->node_uniform.joint_matrix[2] = mesh->node_uniform.joint_matrix[1] * mesh->node_uniform.joint_matrix[2];
     //mesh->node_uniform.joint_matrix[2] = glm::translate(mesh->node_uniform.joint_matrix[2],glm::vec3(0,0,2));
      mesh->node_uniform.joint_matrix[3] = mesh->node_uniform.joint_matrix[2] * mesh->node_uniform.joint_matrix[3];
    // mesh->node_uniform.joint_matrix[2] = mesh->node_uniform.joint_matrix[2] * rot2;
      //mesh->node_uniform.joint_matrix[3] = glm::inverse(mesh->node_uniform.joint_matrix[2] * mesh->node_uniform.joint_matrix[3]) * mesh->node_uniform.joint_matrix[2] * mesh->node_uniform.joint_matrix[3] * mesh->skins[0]->inverse_bind_matrix[3];
      
    //glm::mat4 global_inverse = glm::inverse(NodeManager::get_global_matrix(node));
    //mesh->node_uniform.joint_matrix[3] = mesh->skins[0]->inverse_bind_matrix[3];
}

void Skeletal::load_skin(EMesh* mesh, tinygltf::Model &gltf_model){
    for(tinygltf::Skin &source_skin: gltf_model.skins){
        Skin *new_skin = new Skin{};
        if(source_skin.skeleton > -1){
            new_skin->skeleton_root = node_from_index(mesh, source_skin.skeleton);
        }
        for(int joint_index : source_skin.joints){
            Node* node = node_from_index(mesh, joint_index);
            if(node)
                new_skin->joints.push_back(node);
        }
        if(source_skin.inverseBindMatrices > -1){
            const tinygltf::Accessor &accessor = gltf_model.accessors[source_skin.inverseBindMatrices];

            const tinygltf::BufferView &bufferView = gltf_model.bufferViews[accessor.bufferView];
            
            const tinygltf::Buffer &buffer = gltf_model.buffers[bufferView.buffer];
            
            new_skin->inverse_bind_matrix.resize(accessor.count);
            
            memcpy(new_skin->inverse_bind_matrix.data(),
                     &buffer.data[accessor.byteOffset + bufferView.byteOffset],
                     accessor.count * sizeof(glm::mat4));

        }

        mesh->skins.push_back(new_skin);
    }
    
}

Node* Skeletal::find_node(Node* parent, uint32_t index){
    Node* node_found = nullptr;
    if(parent->index == index)
        return parent;
    for(auto& child : parent->children){
        node_found = find_node(child,index);
        if(node_found)
            break;
    }
    return node_found;
}

Node* Skeletal::node_from_index(EMesh* mesh, uint32_t index){
    Node* node_found = nullptr;
    for(auto &node : mesh->nodes){
        node_found = find_node(node,index);
        if(node_found)
            break;
    }
    return node_found;
}


void Skeletal::load_node(EMesh* mesh, node_load_data& node_data) {
    Node *new_node = new Node{};
    new_node->parent = node_data.parent;
    new_node->matrix = glm::mat4(1.0f);
    new_node->skin_index = node_data.gltf_node->skin;
    new_node->index = node_data.index;
    new_node->name = node_data.gltf_node->name;
    
    //some nodes do not contain transform information
    if(node_data.gltf_node->translation.size() == 3)
        new_node->Translation = glm::make_vec3(node_data.gltf_node->translation.data());

    if(node_data.gltf_node->rotation.size() == 4)
        new_node->Rotation = glm::make_quat(node_data.gltf_node->rotation.data());

    if(node_data.gltf_node->matrix.size() == 16)
        new_node->matrix = glm::make_mat4x4(node_data.gltf_node->matrix.data());

    int children_count = node_data.gltf_node->children.size();
   

    if( children_count > 0){
        for(size_t i = 0;i < children_count ;i++){
            //load_node(new_node,gltf_node.children[i],gltf_model.nodes[gltf_node.children[i]]);
        }
    }
    if(node_data.gltf_node->mesh > -1){
        new_node->mesh = mesh;
    }
}
