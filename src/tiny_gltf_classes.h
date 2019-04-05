#ifndef _TINY_GLTF_CLASSES
#define _TINY_GLTF_CLASSES
// Simple class to represent JSON object
class Value {
 public:
  typedef std::vector<Value> Array;
  typedef std::map<std::string, Value> Object;

  Value() : type_(NULL_TYPE) {}

  explicit Value(bool b) : type_(BOOL_TYPE) { boolean_value_ = b; }
  explicit Value(int i) : type_(INT_TYPE) { int_value_ = i; }
  explicit Value(double n) : type_(NUMBER_TYPE) { number_value_ = n; }
  explicit Value(const std::string &s) : type_(STRING_TYPE) {
    string_value_ = s;
  }
  explicit Value(const unsigned char *p, size_t n) : type_(BINARY_TYPE) {
    binary_value_.resize(n);
    memcpy(binary_value_.data(), p, n);
  }
  explicit Value(const Array &a) : type_(ARRAY_TYPE) {
    array_value_ = Array(a);
  }
  explicit Value(const Object &o) : type_(OBJECT_TYPE) {
    object_value_ = Object(o);
  }

  char Type() const { return static_cast<const char>(type_); }

  bool IsBool() const { return (type_ == BOOL_TYPE); }

  bool IsInt() const { return (type_ == INT_TYPE); }

  bool IsNumber() const { return (type_ == NUMBER_TYPE); }

  bool IsString() const { return (type_ == STRING_TYPE); }

  bool IsBinary() const { return (type_ == BINARY_TYPE); }

  bool IsArray() const { return (type_ == ARRAY_TYPE); }

  bool IsObject() const { return (type_ == OBJECT_TYPE); }

  // Accessor
  template <typename T>
  const T &Get() const;
  template <typename T>
  T &Get();

  // Lookup value from an array
  const Value &Get(int idx) const {
    static Value null_value;
    assert(IsArray());
    assert(idx >= 0);
    return (static_cast<size_t>(idx) < array_value_.size())
               ? array_value_[static_cast<size_t>(idx)]
               : null_value;
  }

  // Lookup value from a key-value pair
  const Value &Get(const std::string &key) const {
    static Value null_value;
    assert(IsObject());
    Object::const_iterator it = object_value_.find(key);
    return (it != object_value_.end()) ? it->second : null_value;
  }

  size_t ArrayLen() const {
    if (!IsArray()) return 0;
    return array_value_.size();
  }

  // Valid only for object type.
  bool Has(const std::string &key) const {
    if (!IsObject()) return false;
    Object::const_iterator it = object_value_.find(key);
    return (it != object_value_.end()) ? true : false;
  }

  // List keys
  std::vector<std::string> Keys() const {
    std::vector<std::string> keys;
    if (!IsObject()) return keys;  // empty

    for (Object::const_iterator it = object_value_.begin();
         it != object_value_.end(); ++it) {
      keys.push_back(it->first);
    }

    return keys;
  }

  size_t Size() const { return (IsArray() ? ArrayLen() : Keys().size()); }

  bool operator==(const tinygltf::Value &other) const;

 protected:
  int type_;

  int int_value_;
  double number_value_;
  std::string string_value_;
  std::vector<unsigned char> binary_value_;
  Array array_value_;
  Object object_value_;
  bool boolean_value_;
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#define TINYGLTF_VALUE_GET(ctype, var)            \
  template <>                                     \
  inline const ctype &Value::Get<ctype>() const { \
    return var;                                   \
  }                                               \
  template <>                                     \
  inline ctype &Value::Get<ctype>() {             \
    return var;                                   \
  }
TINYGLTF_VALUE_GET(bool, boolean_value_)
TINYGLTF_VALUE_GET(double, number_value_)
TINYGLTF_VALUE_GET(int, int_value_)
TINYGLTF_VALUE_GET(std::string, string_value_)
TINYGLTF_VALUE_GET(std::vector<unsigned char>, binary_value_)
TINYGLTF_VALUE_GET(Value::Array, array_value_)
TINYGLTF_VALUE_GET(Value::Object, object_value_)
#undef TINYGLTF_VALUE_GET

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++98-compat"
#pragma clang diagnostic ignored "-Wpadded"
#endif

/// Agregate object for representing a color
using ColorValue = std::array<double, 4>;

struct Parameter {
  bool bool_value = false;
  bool has_number_value = false;
  std::string string_value;
  std::vector<double> number_array;
  std::map<std::string, double> json_double_value;
  double number_value = 0.0;
  // context sensitive methods. depending the type of the Parameter you are
  // accessing, these are either valid or not
  // If this parameter represent a texture map in a material, will return the
  // texture index

  /// Return the index of a texture if this Parameter is a texture map.
  /// Returned value is only valid if the parameter represent a texture from a
  /// material
  int TextureIndex() const {
    const auto it = json_double_value.find("index");
    if (it != std::end(json_double_value)) {
      return int(it->second);
    }
    return -1;
  }

  /// Return the index of a texture coordinate set if this Parameter is a
  /// texture map. Returned value is only valid if the parameter represent a
  /// texture from a material
  int TextureTexCoord() const {
    const auto it = json_double_value.find("texCoord");
    if (it != std::end(json_double_value)) {
      return int(it->second);
    }
    return 0;
  }

  /// Material factor, like the roughness or metalness of a material
  /// Returned value is only valid if the parameter represent a texture from a
  /// material
  double Factor() const { return number_value; }

  /// Return the color of a material
  /// Returned value is only valid if the parameter represent a texture from a
  /// material
  ColorValue ColorFactor() const {
    return {
        {// this agregate intialize the std::array object, and uses C++11 RVO.
         number_array[0], number_array[1], number_array[2],
         (number_array.size() > 3 ? number_array[3] : 1.0)}};
  }

  bool operator==(const Parameter &) const;
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
#endif

typedef std::map<std::string, Parameter> ParameterMap;
typedef std::map<std::string, Value> ExtensionMap;

struct AnimationChannel {
  int sampler;              // required
  int target_node;          // required (index of the node to target)
  std::string target_path;  // required in ["translation", "rotation", "scale",
                            // "weights"]
  Value extras;

  AnimationChannel() : sampler(-1), target_node(-1) {}
  bool operator==(const AnimationChannel &) const;
};

struct AnimationSampler {
  int input;                  // required
  int output;                 // required
  std::string interpolation;  // in ["LINEAR", "STEP", "CATMULLROMSPLINE",
                              // "CUBICSPLINE"], default "LINEAR"
  Value extras;

  AnimationSampler() : input(-1), output(-1), interpolation("LINEAR") {}
  bool operator==(const AnimationSampler &) const;
};

struct Animation {
  std::string name;
  std::vector<AnimationChannel> channels;
  std::vector<AnimationSampler> samplers;
  Value extras;

  bool operator==(const Animation &) const;
};

struct Skin {
  std::string name;
  int inverseBindMatrices;  // required here but not in the spec
  int skeleton;             // The index of the node used as a skeleton root
  std::vector<int> joints;  // Indices of skeleton nodes

  Skin() {
    inverseBindMatrices = -1;
    skeleton = -1;
  }
  bool operator==(const Skin &) const;
};

struct Sampler {
  std::string name;
  int minFilter;  // ["NEAREST", "LINEAR", "NEAREST_MIPMAP_LINEAR",
                  // "LINEAR_MIPMAP_NEAREST", "NEAREST_MIPMAP_LINEAR",
                  // "LINEAR_MIPMAP_LINEAR"]
  int magFilter;  // ["NEAREST", "LINEAR"]
  int wrapS;      // ["CLAMP_TO_EDGE", "MIRRORED_REPEAT", "REPEAT"], default
                  // "REPEAT"
  int wrapT;      // ["CLAMP_TO_EDGE", "MIRRORED_REPEAT", "REPEAT"], default
                  // "REPEAT"
  int wrapR;      // TinyGLTF extension
  Value extras;

  Sampler()
      : minFilter(TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_LINEAR),
        magFilter(TINYGLTF_TEXTURE_FILTER_LINEAR),
        wrapS(TINYGLTF_TEXTURE_WRAP_REPEAT),
        wrapT(TINYGLTF_TEXTURE_WRAP_REPEAT),
        wrapR(TINYGLTF_TEXTURE_WRAP_REPEAT) {}
  bool operator==(const Sampler &) const;
};

struct Image {
  std::string name;
  int width;
  int height;
  int component;
  int bits;        // bit depth per channel. 8(byte), 16 or 32.
  int pixel_type;  // pixel type(TINYGLTF_COMPONENT_TYPE_***). usually
                   // UBYTE(bits = 8) or USHORT(bits = 16)
  std::vector<unsigned char> image;
  int bufferView;        // (required if no uri)
  std::string mimeType;  // (required if no uri) ["image/jpeg", "image/png",
                         // "image/bmp", "image/gif"]
  std::string uri;       // (required if no mimeType)
  Value extras;
  ExtensionMap extensions;

  // When this flag is true, data is stored to `image` in as-is format(e.g. jpeg
  // compressed for "image/jpeg" mime) This feature is good if you use custom
  // image loader function. (e.g. delayed decoding of images for faster glTF
  // parsing) Default parser for Image does not provide as-is loading feature at
  // the moment. (You can manipulate this by providing your own LoadImageData
  // function)
  bool as_is;

  Image() : as_is(false) {
    bufferView = -1;
    width = -1;
    height = -1;
    component = -1;
  }
  bool operator==(const Image &) const;
};

struct Texture {
  std::string name;

  int sampler;
  int source;
  Value extras;
  ExtensionMap extensions;

  Texture() : sampler(-1), source(-1) {}
  bool operator==(const Texture &) const;
};

// Each extension should be stored in a ParameterMap.
// members not in the values could be included in the ParameterMap
// to keep a single material model
struct Material {
  std::string name;

  ParameterMap values;            // PBR metal/roughness workflow
  ParameterMap additionalValues;  // normal/occlusion/emissive values

  ExtensionMap extensions;
  Value extras;

  bool operator==(const Material &) const;
};

struct BufferView {
  std::string name;
  int buffer;         // Required
  size_t byteOffset;  // minimum 0, default 0
  size_t byteLength;  // required, minimum 1
  size_t byteStride;  // minimum 4, maximum 252 (multiple of 4), default 0 =
                      // understood to be tightly packed
  int target;         // ["ARRAY_BUFFER", "ELEMENT_ARRAY_BUFFER"]
  Value extras;
  bool dracoDecoded;  // Flag indicating this has been draco decoded

  BufferView() : byteOffset(0), byteStride(0), dracoDecoded(false) {}
  bool operator==(const BufferView &) const;
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
  Value extras;

  std::vector<double> minValues;  // optional
  std::vector<double> maxValues;  // optional

  struct {
    int count;
    bool isSparse;
    struct {
      int byteOffset;
      int bufferView;
      int componentType;  // a TINYGLTF_COMPONENT_TYPE_ value
    } indices;
    struct {
      int bufferView;
      int byteOffset;
    } values;
  } sparse;

  ///
  /// Utility function to compute byteStride for a given bufferView object.
  /// Returns -1 upon invalid glTF value or parameter configuration.
  ///
  int ByteStride(const BufferView &bufferViewObject) const {
    if (bufferViewObject.byteStride == 0) {
      // Assume data is tightly packed.
      int componentSizeInBytes =
          GetComponentSizeInBytes(static_cast<uint32_t>(componentType));
      if (componentSizeInBytes <= 0) {
        return -1;
      }

      int typeSizeInBytes = GetTypeSizeInBytes(static_cast<uint32_t>(type));
      if (typeSizeInBytes <= 0) {
        return -1;
      }

      return componentSizeInBytes * typeSizeInBytes;
    } else {
      // Check if byteStride is a mulple of the size of the accessor's component
      // type.
      int componentSizeInBytes =
          GetComponentSizeInBytes(static_cast<uint32_t>(componentType));
      if (componentSizeInBytes <= 0) {
        return -1;
      }

      if ((bufferViewObject.byteStride % uint32_t(componentSizeInBytes)) != 0) {
        return -1;
      }
      return static_cast<int>(bufferViewObject.byteStride);
    }

    return 0;
  }

  Accessor() {
    bufferView = -1;
    sparse.isSparse = false;
  }
  bool operator==(const tinygltf::Accessor &) const;
};

struct PerspectiveCamera {
  double aspectRatio;  // min > 0
  double yfov;         // required. min > 0
  double zfar;         // min > 0
  double znear;        // required. min > 0

  PerspectiveCamera()
      : aspectRatio(0.0),
        yfov(0.0),
        zfar(0.0)  // 0 = use infinite projecton matrix
        ,
        znear(0.0) {}
  bool operator==(const PerspectiveCamera &) const;

  ExtensionMap extensions;
  Value extras;
};

struct OrthographicCamera {
  double xmag;   // required. must not be zero.
  double ymag;   // required. must not be zero.
  double zfar;   // required. `zfar` must be greater than `znear`.
  double znear;  // required

  OrthographicCamera() : xmag(0.0), ymag(0.0), zfar(0.0), znear(0.0) {}
  bool operator==(const OrthographicCamera &) const;

  ExtensionMap extensions;
  Value extras;
};

struct Camera {
  std::string type;  // required. "perspective" or "orthographic"
  std::string name;

  PerspectiveCamera perspective;
  OrthographicCamera orthographic;

  Camera() {}
  bool operator==(const Camera &) const;

  ExtensionMap extensions;
  Value extras;
};

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
  ExtensionMap extensions;
  Value extras;

  Primitive() {
    material = -1;
    indices = -1;
  }
  bool operator==(const Primitive &) const;
};

struct Mesh {
  std::string name;
  std::vector<Primitive> primitives;
  std::vector<double> weights;  // weights to be applied to the Morph Targets
  std::vector<std::map<std::string, int> > targets;
  ExtensionMap extensions;
  Value extras;

  bool operator==(const Mesh &) const;
};

class Node {
 public:
  Node() : camera(-1), skin(-1), mesh(-1) {}

  Node(const Node &rhs) {
    camera = rhs.camera;

    name = rhs.name;
    skin = rhs.skin;
    mesh = rhs.mesh;
    children = rhs.children;
    rotation = rhs.rotation;
    scale = rhs.scale;
    translation = rhs.translation;
    matrix = rhs.matrix;
    weights = rhs.weights;

    extensions = rhs.extensions;
    extras = rhs.extras;
  }
  ~Node() {}
  bool operator==(const Node &) const;

  int camera;  // the index of the camera referenced by this node

  std::string name;
  int skin;
  int mesh;
  std::vector<int> children;
  std::vector<double> rotation;     // length must be 0 or 4
  std::vector<double> scale;        // length must be 0 or 3
  std::vector<double> translation;  // length must be 0 or 3
  std::vector<double> matrix;       // length must be 0 or 16
  std::vector<double> weights;  // The weights of the instantiated Morph Target

  ExtensionMap extensions;
  Value extras;
};

struct Buffer {
  std::string name;
  std::vector<unsigned char> data;
  std::string
      uri;  // considered as required here but not in the spec (need to clarify)
  Value extras;

  bool operator==(const Buffer &) const;
};

struct Asset {
  std::string version;  // required
  std::string generator;
  std::string minVersion;
  std::string copyright;
  ExtensionMap extensions;
  Value extras;

  bool operator==(const Asset &) const;
};

struct Scene {
  std::string name;
  std::vector<int> nodes;

  ExtensionMap extensions;
  Value extras;

  bool operator==(const Scene &) const;
};

struct Light {
  std::string name;
  std::vector<double> color;
  std::string type;

  bool operator==(const Light &) const;
};

class Model {
 public:
  Model() {}
  ~Model() {}
  bool operator==(const Model &) const;

  std::vector<Accessor> accessors;
  std::vector<Animation> animations;
  std::vector<Buffer> buffers;
  std::vector<BufferView> bufferViews;
  std::vector<Material> materials;
  std::vector<Mesh> meshes;
  std::vector<Node> nodes;
  std::vector<Texture> textures;
  std::vector<Image> images;
  std::vector<Skin> skins;
  std::vector<Sampler> samplers;
  std::vector<Camera> cameras;
  std::vector<Scene> scenes;
  std::vector<Light> lights;
  ExtensionMap extensions;

  int defaultScene;
  std::vector<std::string> extensionsUsed;
  std::vector<std::string> extensionsRequired;

  Asset asset;

  Value extras;
};

enum SectionCheck {
  NO_REQUIRE = 0x00,
  REQUIRE_SCENE = 0x01,
  REQUIRE_SCENES = 0x02,
  REQUIRE_NODES = 0x04,
  REQUIRE_ACCESSORS = 0x08,
  REQUIRE_BUFFERS = 0x10,
  REQUIRE_BUFFER_VIEWS = 0x20,
  REQUIRE_ALL = 0x3f
};

#endif