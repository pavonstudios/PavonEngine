#ifndef ANDROID_HELPER
#define  ANDROID_HELPER

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

const char vertex_src [] =
"                                        \
    attribute vec3        position;       \
    attribute vec2          inUV;         \
    uniform mat4          MVP;              \
    varying vec2 UV;                   \
                                         \
   void main()                           \
   {                                     \
      gl_Position = MVP * vec4(position,1.0);   \
      UV = inUV;              \
   }                                     \
";
 
 
const char fragment_src [] =
"     \
    precision mediump float;                                                 \
    uniform sampler2D texture_sampler;                  \
    varying vec2 UV;                                         \
                                                       \
   void  main()                                        \
   {                                                            \
      gl_FragColor = texture2D(texture_sampler,UV);                   \
                                                             \
   }                                                   \
";



const float vertexArray[] = {
   0.0,  0.5,  0.0,
  -0.5,  0.0,  0.0,
   0.0, -0.5,  0.0,
   0.5,  0.0,  0.0,
   0.0,  0.5,  0.0 
};

static EGLint const attribute_list[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_BLUE_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_RED_SIZE, 8,
        EGL_CONFORMANT, EGL_OPENGL_ES2_BIT,
        EGL_NONE
};
static const EGLint GiveMeGLES2[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
};



#endif