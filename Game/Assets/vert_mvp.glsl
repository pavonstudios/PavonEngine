#version 100
attribute mediump vec3        	position;
attribute mediump vec3			inColor;
attribute mediump vec2          inUV;
varying mediump vec2 v_TexCoord;
varying mediump vec3 color;
uniform mat4          MVP;
void main(){
gl_Position = MVP * vec4(position,1.0);
v_TexCoord = inUV;
color = inColor;}