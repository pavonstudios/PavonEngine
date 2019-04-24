#version 100
precision highp float;
varying vec2 v_TexCoord;	
varying vec3 color;	
uniform sampler2D texture_sampler;               												
void main(){
    float z = float(5) + v_TexCoord.x;
    gl_FragColor = vec4(v_TexCoord.x,v_TexCoord.y,z,1.0);}