#include "objects.h"
namespace engine{
	struct EMesh;
	struct Skin;

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
		uint32_t bone_index;   
		glm::mat4 global_matrix = glm::mat4(1.0); 
		
	};

	struct Skin {
			Node *skeleton_root = nullptr;
			std::vector<glm::mat4> inverse_bind_matrix;
			std::vector<Node*> joints;
	};
	
	struct node_load_data{
		tinygltf::Model* gltf_model;
		engine::Node *parent; 
		uint32_t index;
		tinygltf::Node *gltf_node;
	};

	struct SkeletalMesh{
		std::vector<Node*> nodes;
		std::vector<Node*> linear_nodes;
		Skin* skin;
		struct NodeUniform node_uniform;
	};
	

	struct AnimationChannel {
		Node* node;
		uint32 sampler_index;
	};

	struct AnimationSampler{
		
	};

	struct Animation{
		AnimationChannel channels;
	};


	class Skeletal{
	public:
		static void load_node(EMesh* mesh, node_load_data& node_data);    
		static Node* node_from_index(EMesh* mesh, uint32_t index);
		static Node* find_node(Node* parent, uint32_t index);
		static void load_skin(EMesh* mesh, tinygltf::Model &gltf_model);
		static void load_data(EMesh* mesh);
		static void update_joints_matrix(EMesh* mesh, Node* node);
		static Node* node_by_name(EMesh* mesh, const char* name);
		static void update_joints_nodes(EMesh* mesh);
		static void update_joint_matrix(Node* node);
	};

	class NodeManager{
		public:
			static void create_nodes_index(EMesh* mesh);
			static void update(Node* node);
			static void update(EMesh* mesh, Node*  node);
			static glm::mat4 get_local_matrix(Node* node);
			static glm::mat4 get_global_matrix(Node* node);
			static glm::mat4 get_global_matrix_simple(Node* node);
	};

}