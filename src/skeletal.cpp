#include "objects.h"
#include "engine.h"
using namespace engine;
void Skeletal::update_joints_nodes(EMesh* mesh){
    
    Skin* skin = mesh->skins[0];
    size_t joints_number = skin->joints.size();
    mesh->node_uniform.joint_count = (float)joints_number;
    



    for(int i = 0; i < skin->joints.size(); i++){
        Node* joint = skin->joints[i];
        Skeletal::update_joint_matrix(joint);
        
        glm::mat4 bind_mat = NodeManager::get_global_matrix(joint);
        glm::mat joint_mat = 

            joint->global_matrix; //* skin->inverse_bind_matrix[i];


        mesh->node_uniform.joint_matrix[i] = joint_mat;
    } 

}

void Skeletal::load_data(EMesh* mesh){
    int node_count = mesh->gltf_model.nodes.size();
    for(size_t i = 0; i < node_count;i++){
        NodeLoadData load_data = {};
        load_data.gltf_model = &mesh->gltf_model;
        load_data.gltf_node = &mesh->gltf_model.nodes[i];
        load_data.index = i;
        load_data.parent = nullptr;
        Skeletal::load_node(mesh,load_data);
    }

  
    Skeletal::load_skin(mesh, mesh->gltf_model);

    mesh->skeletal = new SkeletalMesh;
    Skeletal::load_animation(mesh->skeletal,mesh->gltf_model);
    mesh->skeletal->nodes = mesh->nodes;
    mesh->skeletal->linear_nodes = mesh->linear_nodes;
    mesh->skeletal->mesh = mesh; 


    Skeletal::update_joints_nodes(mesh);


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
    glm::mat4 local = glm::translate(glm::mat4(1.0f),node->Translation) * glm::mat4(node->Rotation) * node->rot_mat;
    return local;
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

 void Skeletal::load_node(EMesh* mesh, NodeLoadData& node_data){
     
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
            mesh->nodes[node_data.gltf_node->children[i]]->parent = new_node;
        }
        
    }
    if(node_data.gltf_node->mesh > -1){
        new_node->mesh = mesh;
    }
    if(node_data.parent){
        node_data.parent->children.push_back(new_node);       
    }else{
         mesh->nodes.push_back(new_node);
    }

    mesh->linear_nodes.push_back(new_node);
 }


void Skeletal::load_animation(SkeletalMesh* skeletal, tinygltf::Model &gltf_model){
    for(auto& anim : gltf_model.animations){
        Animation new_animation;

        for(auto& sampler : anim.samplers){
            AnimationSampler new_sampler{};

            {
                //inputs
                const tinygltf::Accessor &accessor = gltf_model.accessors[sampler.input];
                const tinygltf::BufferView &bufferView = gltf_model.bufferViews[accessor.bufferView];
                const tinygltf::Buffer &buffer = gltf_model.buffers[bufferView.buffer];

                
                const void *dataPtr = &buffer.data[accessor.byteOffset + bufferView.byteOffset];
                const float *buf = static_cast<const float*>(dataPtr);
                for (size_t index = 0; index < accessor.count; index++) {
                    new_sampler.inputs.push_back(buf[index]);
                }
            }


            //outputs
            const tinygltf::Accessor &accessor = gltf_model.accessors[sampler.output];
            const tinygltf::BufferView &bufferView = gltf_model.bufferViews[accessor.bufferView];
            const tinygltf::Buffer &buffer = gltf_model.buffers[bufferView.buffer];

            const void *dataPtr = &buffer.data[accessor.byteOffset + bufferView.byteOffset];

            switch (accessor.type) {
                case TINYGLTF_TYPE_VEC3: {
                    const glm::vec3 *buf = static_cast<const glm::vec3*>(dataPtr);
                    for (size_t index = 0; index < accessor.count; index++) {
                        new_sampler.outputs_vec4.push_back(glm::vec4(buf[index], 0.0f));
                    }
                    break;
                }
                case TINYGLTF_TYPE_VEC4: {
                    const glm::vec4 *buf = static_cast<const glm::vec4*>(dataPtr);
                    for (size_t index = 0; index < accessor.count; index++) {
                        new_sampler.outputs_vec4.push_back(buf[index]);
                    }
                    break;
                }
                default: {
                    std::cout << "unknown type" << std::endl;
                    break;
                }
            }
            new_animation.samplers.push_back(new_sampler);

        }

        skeletal->animations.push_back(new_animation);
    }
}

void Skeletal::reset_animations(std::vector<SkeletalMesh*> skeletals){
    for(auto* skeletal : skeletals){
          EMesh* mesh = skeletal->mesh;
         Node* node = Skeletal::node_by_name(mesh,"bone2");

        quat new_quat{};
        node->Rotation = new_quat;
    }
}

void Skeletal::play_animations(std::vector<SkeletalMesh*> skeletals, float time){
    //std::cout << "play\n";
    for(auto* skeletal : skeletals){
        mat4 model_space = mat4(1.0);
        glm::quat quat1;
        AnimationSampler sampler = skeletal->animations[0].samplers[0];

        quat1.x = skeletal->animations[0].samplers[0].outputs_vec4[4].x;
        quat1.y = skeletal->animations[0].samplers[0].outputs_vec4[4].y;
        quat1.z = skeletal->animations[0].samplers[0].outputs_vec4[4].z;
        quat1.w = skeletal->animations[0].samplers[0].outputs_vec4[4].w;     
       
        EMesh* mesh = skeletal->mesh;
        			
		
        //node->Translation = vec3(skeletal->animations[0].samplers[0].outputs_vec4[4]);

       for(size_t i = 0; i < sampler.inputs.size() - 1 ; i++ ){
           if( (time >= sampler.inputs[i])  && ( time <= sampler.inputs[i + 1] ) ){

                Node* node = Skeletal::node_by_name(mesh,"bone2");

                node->Rotation = quat1;
           }

       }

      //   mat4 rot = glm::mat4(quat1);
        //node->rot_mat = rot;
    }
}

void Skeletal::update_joint_vertices_data(Engine* engine){
    EMesh* mesh = engine->helpers[0];
    mesh->vertices.clear();

    for(auto* node : engine->skeletal_meshes[0]->skins[0]->joints){
        mat4 local = NodeManager::get_global_matrix(node);
        local = engine->skeletal_meshes[0]->model_matrix * local;
        Vertex vert {};
        vec3 position =  vec3(local[3]);
        vert.pos = position;
        mesh->vertices.push_back(vert);
    }
   
    #ifdef ES2
    glBindBuffer(GL_ARRAY_BUFFER,mesh->vertex_buffer);
    //glBufferSubData(mesh->vertex_buffer,0,mesh->vertices.size() * sizeof(Vertex),mesh->vertices.data());
    glBufferData(GL_ARRAY_BUFFER,mesh->vertices.size() * sizeof(Vertex),mesh->vertices.data(),GL_STATIC_DRAW);

    #endif
}

void Skeletal::create_bones_vertices(Engine* engine){
   
    EMesh* triangle = new EMesh();

    for(auto* node : engine->skeletal_meshes[0]->skins[0]->joints){
        mat4 local = NodeManager::get_global_matrix(node);
        local = engine->skeletal_meshes[0]->model_matrix * local;
        Vertex vert {};
        vec3 position =  vec3(local[3]);
        vert.pos = position;
        triangle->vertices.push_back(vert);

    }

    
    triangle->data_shader.fragment_shader_path = "Game/Assets/shaders/gles/blue.glsl";

    
    triangle->data_shader.vertex_shader_path = "Game/Assets/shaders/gles/vert_mvp.glsl";
   
    triangle->type = -1;
    triangle->bIsGUI = false;
    triangle->name = "joints";   
    

    #if defined (ES2) || defined (ANDROID) 
        engine->linear_meshes.push_back(triangle);
        engine->helpers.push_back(triangle);        
    #endif
}