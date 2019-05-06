#version 100
precision highp float;
uniform sampler2D texture_sampler;
varying vec3 color;
varying vec2 v_TexCoord;
void main(){gl_FragColor = texture2D(texture_sampler,v_TexCoord);}//end