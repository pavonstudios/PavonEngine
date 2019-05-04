#version 100
precision mediump float;
uniform sampler2D texture_sampler;
varying vec2 v_TexCoord;
varying vec3 color;

void main()
{
vec4 texture = texture2D(texture_sampler,v_TexCoord);
gl_FragColor = vec4(color.z,color.x,color.y,1.0);
}
//end