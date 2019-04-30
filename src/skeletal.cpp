#include "objects.h"
using namespace engine;
void Skeletal::load_data(EMesh* mesh){
    int node_count = mesh->gltf_model.nodes.size();
    for(size_t i = 0; i < node_count;i++){
        mesh->load_node(nullptr,i,mesh->gltf_model.nodes[i]);
    }

  
    Skeletal::load_skin(mesh, mesh->gltf_model);

 
    NodeManager::create_nodes_index(mesh);

    Node* upper_arm_node = Skeletal::node_by_name(mesh, "upper_arm");
     Node* root = Skeletal::node_by_name(mesh, "Bone");
    glm::mat4 move_up = glm::translate(glm::mat4(1.0),glm::vec3(0,0,2));
     glm::mat4 rot = glm::rotate(glm::mat4(1.0),glm::radians(90.f),glm::vec3(0,1,0));
    //upper_arm_node->matrix = rot * inverse(mesh->model_matrix);
    //root->matrix = move_up;
     Skeletal::update_joints_nodes(mesh);
   // mesh->node_uniform.joint_matrix[2] = glm::translate(glm::mat4(1.0),glm::vec3(0,0,2));

}

void Skeletal::update_joint_matrix(Node* node){
    glm::mat4 joint_mat = glm::mat4(1.0);
    if(node->parent){
        joint_mat = node->parent->global_matrix * node->matrix;
    }
    node->global_matrix = joint_mat;
}

void NodeManager::create_nodes_index(EMesh* mesh){
    int index = 0;
    Skin* skin = mesh->skins[0];
    int joint_count = skin->joints.size();
    for(int i = 0;i<joint_count;i++){
        Node* joint_node = skin->joints[i];
        joint_node->bone_index = index;
        index++;
    }
}
void Skeletal::update_joints_nodes(EMesh* mesh){
    
    Skin* skin = mesh->skins[0];
    size_t joints_number = skin->joints.size();
    mesh->node_uniform.joint_count = (float)joints_number;
    
    for(int i = 0; i < skin->joints.size(); i++){
        Node* joint = skin->joints[i];
        Skeletal::update_joint_matrix(joint);
        glm::mat4 rot = glm::rotate(skin->inverse_bind_matrix[i],glm::radians(90.f),glm::vec3(1,0,0));
        glm::mat4 bind_mat = NodeManager::get_global_matrix(joint);
        glm::mat joint_mat = 
            //glm::inverse(mesh->model_matrix) *
            //joint->matrix *
            joint->global_matrix;//* glm::inverse(mesh->model_matrix);// * skin->inverse_bind_matrix[i];
            //glm::inverse(bind_mat);
            rot;
        mesh->node_uniform.joint_matrix[i] = joint_mat;
    } 

        mat4 model_space = mat4(1.0);
        mat4 move = translate(model_space,vec3(0,0,1));
        model_space = model_space * inverse(mesh->model_matrix);
        mat4 rot = rotate(mat4(1.0),radians(90.f),vec3(0,1,0));
        mat4 transform = move * rot;
        model_space = inverse(mesh->node_uniform.joint_matrix[2]) * transform;
        mesh->node_uniform.joint_matrix[2] = model_space;
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

            Skeletal::update_joints_nodes(mesh);
        }
    }
    for(auto& child : node->children){
        NodeManager::update(child);
    }
}

void NodeManager::update(EMesh* mesh, Node* node){  
    
    
    /* for(auto& child : node->children){
        NodeManager::update(mesh,child);
    } */
    Skeletal::update_joints_nodes(mesh);
        
}

glm::mat4 NodeManager::get_local_matrix(Node* node){
    glm::mat4 local = glm::translate(glm::mat4(1.0f),node->Translation) * glm::mat4(node->Rotation) * node->matrix;
    glm::mat4 local_location = glm::translate(glm::mat4(1.0f),node->Translation);
    glm::mat4 reseted_local_position = glm::translate(node->matrix,glm::vec3(0,0,0));
    node->matrix = reseted_local_position;
    return node->matrix;
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
glm::mat4 NodeManager::get_global_matrix_simple(Node* node){
    glm::mat4 local_matrix = get_local_matrix(node);
    Node* node_parent = node->parent;

    glm::mat4 global_mat;
    if(node_parent)       
        global_mat = get_local_matrix(node_parent) * local_matrix;
    else
    {
        global_mat = local_matrix;
    }
     
   
    return global_mat;

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

Node* Skeletal::node_by_name(EMesh* mesh, const char* name ){
    Node* node_found = nullptr;
    for(auto node : mesh->nodes){
        if(node->name == name){
            node_found = node;
        }        
        if(node_found)
            break;
    }
    if(!node_found)
        std::cout << "node not found : " << std::string(name) << std::endl;
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
