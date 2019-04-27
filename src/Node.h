
struct Node{
        Node *parent;
        uint32_t index;
        glm::vec3 Translation{};
        glm::quat Rotation{};
        glm::mat4 matrix;
        std::vector<Node*> children;
        EMesh* mesh;
        Skin* skin;
        int32_t skin_index = -1;
        std::string name;
        
        void rotate(){
            glm::vec3 movement = glm::vec3(0,0.0001,0);
            //update();
            if(skin){
                this->Rotation = glm::quat(glm::vec3(0,0,0));
                mesh->node_uniform.matrix = glm::mat4(1.0);
                size_t joints_number = skin->joints.size();
                mesh->node_uniform.joint_count = (float)joints_number;
                skin->joints[1]->Translation += movement;
                mesh->node_uniform.joint_matrix[1] = glm::inverse(skin->joints[1]->get_local_matrix()) * 
                                                    (skin->joints[1]->get_local_matrix() * skin->joints[0]->get_local_matrix()) * 
                                                    skin->inverse_bind_matrix[1];
                mesh->node_uniform.joint_matrix[0] = glm::inverse(skin->joints[0]->get_local_matrix()) * 
                                                    skin->joints[0]->get_local_matrix() * 
                                                    skin->inverse_bind_matrix[0];            }
            
        }
        void move_root(){
            glm::vec3 movement = glm::vec3(0,0.0001,0);
            //update();
            if(skin){
                mesh->node_uniform.matrix = glm::mat4(1.0);
                size_t joints_number = skin->joints.size();
                mesh->node_uniform.joint_count = (float)joints_number;
                skin->joints[0]->Translation += movement;
                mesh->node_uniform.joint_matrix[1] = glm::inverse(skin->joints[1]->get_local_matrix()) * 
                                                    (skin->joints[1]->get_local_matrix() * skin->joints[0]->get_local_matrix()) * 
                                                    skin->inverse_bind_matrix[1];
                mesh->node_uniform.joint_matrix[0] = glm::inverse(skin->joints[0]->get_local_matrix()) * 
                                                    skin->joints[0]->get_local_matrix() * 
                                                    skin->inverse_bind_matrix[0];
            }
            
        }
        glm::mat4 get_local_matrix(){
            return glm::translate(glm::mat4(1.0f),Translation) * glm::mat4(Rotation);
        }
        glm::mat4 get_matrix(){
                glm::mat4 local_matrix = get_local_matrix();
                Node* node_parent = this->parent;
                while(node_parent){
                    local_matrix = node_parent->get_local_matrix() * local_matrix;
                    node_parent = node_parent->parent;
                }
                return local_matrix;
        }
        
};

