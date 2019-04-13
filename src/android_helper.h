const char vertex_src [] =
"                                        \
   attribute vec3        position;       \
    attribute vec2          coord;         \
   varying mediump vec2  pos;            \
   uniform vec4          offset;         \
   uniform mat4          MVP;  \
    varying vec2 texcoordOut;          \
                                         \
   void main()                           \
   {                                     \
      gl_Position = MVP * vec4(position,1.0);   \
      pos = position.xy;   \
        texcoordOut = coord;              \
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
      gl_FragColor  =  vec4(0,1,1,1);                   \
                                                             \
   }                                                   \
";


void
print_shader_info_log (
   GLuint  shader      // handle to the shader
)
{
   GLint  length;
 
   glGetShaderiv ( shader , GL_INFO_LOG_LENGTH , &length );
 
   if ( length ) {
      char* buffer  =  new char [ length ];
      glGetShaderInfoLog ( shader , length , NULL , buffer );
      LOGW("shader info %s",buffer);
      //cout << "shader info: " <<  buffer << flush;
      delete [] buffer;
 
      GLint success;
      glGetShaderiv( shader, GL_COMPILE_STATUS, &success );
      if ( success != GL_TRUE )   exit ( 1 );
   }
}

GLuint
load_shader (
   const char  *shader_source,
   GLenum       type
)
{
   GLuint  shader = glCreateShader( type );
 
   glShaderSource  ( shader , 1 , &shader_source , NULL );
   glCompileShader ( shader );
 
    print_shader_info_log ( shader );
 
   return shader;
}

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

GLint
        phase_loc,
        offset_loc,
        position_loc,
        sampler,
        uvposition,
        mvp_loc;
 
 