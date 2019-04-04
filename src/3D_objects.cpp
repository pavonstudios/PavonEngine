#include "3D_objects.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"


#define STB_IMAGE_IMPLEMENTATION

#ifdef GLTF
    #define TINYGLTF_IMPLEMENTATION
    #define STB_IMAGE_WRITE_IMPLEMENTATION
    #include "tiny_gltf.h"
#else
    #include "stb_image.h"
#endif




#ifdef GLTF
using namespace engine;
int Mesh::load_model_gltf(const char* path){
    using namespace tinygltf;
    Model model;
    TinyGLTF loader;
    std::string err;
    std::string warn;

    bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, path);
    //bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, argv[1]); // for binary glTF(.glb)

    if (!warn.empty()) {
    printf("Warn: %s\n", warn.c_str());
    }

    if (!err.empty()) {
    printf("Err: %s\n", err.c_str());
    }

    if (!ret) {
    printf("Failed to parse glTF\n");
    return -1;
    }
    if(ret){
           printf("glb loaded\n");
    }
}
#endif

using namespace engine;
bool Mesh::load_model2(const char * path){

            std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
            std::vector< glm::vec3 > temp_vertices;
            std::vector< glm::vec2 > temp_uvs;
            std::vector< glm::vec3 > temp_normals;

            FILE * file = fopen(path, "r");
            if( file == NULL ){
                printf("Impossible to open the file !\n");
                return false;
            }
            while( 1 ){

                    char lineHeader[128];
                    // read the first word of the line
                    int res = fscanf(file, "%s", lineHeader);
                    if (res == EOF)
                        break; // EOF = End Of File. Quit the loop.

                    // else : parse lineHeader
                    if ( strcmp( lineHeader, "v" ) == 0 ){
                        glm::vec3 vertex;
                        fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
                        temp_vertices.push_back(vertex);
                    }else if ( strcmp( lineHeader, "f" ) == 0 ){
                        std::string vertex1, vertex2, vertex3;
                        unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
                        int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
                        if (matches != 9){
                            printf("File can't be read by our simple parser : ( Try exporting with other options\n");
                            return false;
                        }
                        vertexIndices.push_back(vertexIndex[0]);
                        vertexIndices.push_back(vertexIndex[1]);
                        vertexIndices.push_back(vertexIndex[2]);
                        uvIndices    .push_back(uvIndex[0]);
                        uvIndices    .push_back(uvIndex[1]);
                        uvIndices    .push_back(uvIndex[2]);
                        normalIndices.push_back(normalIndex[0]);
                        normalIndices.push_back(normalIndex[1]);
                        normalIndices.push_back(normalIndex[2]);
                    }

                    for( unsigned int i=0; i<vertexIndices.size(); i++ ){
                        unsigned int vertexIndex = vertexIndices[i];
                        glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
                        simple_vertices.push_back(vertex);
                    }
                    
            }
            return true;
        }
void Mesh::load_model(std::string model_path){
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, model_path.c_str())) {
            throw std::runtime_error(warn + err);
        }

        std::unordered_map<Vertex, uint32_t> uniqueVertices = {};

        for (const auto& shape : shapes) {
            for (const auto& index : shape.mesh.indices) {
                Vertex vertex = {};

                vertex.pos = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                };

                vertex.texCoord = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                };

                vertex.color = {1.0f, 1.0f, 1.0f};

                if (uniqueVertices.count(vertex) == 0) {
                    uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex);
                }

                indices.push_back(uniqueVertices[vertex]);
            }
        }
}

void Mesh::SetLocation(float x, float y, float z){
		Location.x = x;
		Location.y = y;
		Location.z = z;

        glm::mat4 myMatrix = glm::translate(glm::mat4(), glm::vec3(Location.x,Location.y,Location.z));
        glm::vec4 myVector(10.0f, 10.0f, 10.0f, 0.0f);
        glm::vec4 transformedVector = myMatrix * myVector; // guess the result
       // model_matrix = transformedVector;
       model_matrix = glm::translate(model_matrix, glm::vec3(Location.x,Location.y,Location.z));
}

Mesh::~Mesh(){
    
}

void AssetManager::free_image(void * pixels){
    stbi_image_free(pixels);
}
image_size AssetManager::load_and_get_size(std::string texture_path){
    int texWidth, texHeight, texChannels;
    stbi_uc* pixels = stbi_load(texture_path.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        
    
    if (!pixels) {
            throw std::runtime_error("failed to load texture image!");
    }
    image_size size = {texWidth, texHeight, pixels};
    return size;
}