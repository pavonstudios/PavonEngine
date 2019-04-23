#version 100
precision highp float;
varying vec2 v_TexCoord;	
varying vec3 color;	
uniform sampler2D texture_sampler;               												
void main(){
		gl_FragColor = vec4(color,1.0);
}