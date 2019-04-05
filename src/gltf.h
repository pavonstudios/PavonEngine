#ifndef GLTF_H
#define GLTF_H
#include <vector>
#include <string>
#include <map>
namespace loader{

struct Primitive {
  std::map<std::string, int> attributes;  // (required) A dictionary object of
                                          // integer, where each integer
                                          // is the index of the accessor
                                          // containing an attribute.
  int material;  // The index of the material to apply to this primitive
                 // when rendering.
  int indices;   // The index of the accessor that contains the indices.
  int mode;      // one of TINYGLTF_MODE_***
  std::vector<std::map<std::string, int> > targets;  // array of morph targets,
  // where each target is a dict with attribues in ["POSITION, "NORMAL",
  // "TANGENT"] pointing
  // to their corresponding accessors

  //ExtensionMap extensions;
  //Value extras;

  Primitive() {
    material = -1;
    indices = -1;
  }
  
};
struct Mesh {
  std::string name;
  std::vector<Primitive> primitives;
  std::vector<double> weights;  // weights to be applied to the Morph Targets
  std::vector<std::map<std::string, int> > targets;
  //ExtensionMap extensions;
  //Value extras;

  //bool operator==(const Mesh &) const;
};
struct Accessor {
  int bufferView;  // optional in spec but required here since sparse accessor
                   // are not supported
  std::string name;
  size_t byteOffset;
  bool normalized;    // optional.
  int componentType;  // (required) One of TINYGLTF_COMPONENT_TYPE_***
  size_t count;       // required
  int type;           // (required) One of TINYGLTF_TYPE_***   ..
};
struct BufferView {
  std::string name;
  int buffer;         // Required
  size_t byteOffset;  // minimum 0, default 0
  size_t byteLength;  // required, minimum 1
  size_t byteStride;  // minimum 4, maximum 252 (multiple of 4), default 0 =
                      // understood to be tightly packed
  int target;         // ["ARRAY_BUFFER", "ELEMENT_ARRAY_BUFFER"]
  //Value extras;
  bool dracoDecoded;  // Flag indicating this has been draco decoded
  
};
struct Buffer{
	std::string name;
	std::vector<unsigned char> data;
};
struct gltf
{
	std::vector<Buffer> buffers;
	std::vector<Accessor> accessors;
	std::vector<BufferView> bufferViews;
	std::vector<Mesh> meshes;
};



}//end namespace loader
class Loader{
public:
	void load_file(const char* path);
	void load_from_string(loader::gltf gltf, const char* str, unsigned int lenght);

};
#endif