#version 100
precision highp float;
varying vec2 v_TexCoord;	
varying vec3 color;	             												
void main(){
    float z = float(15) + v_TexCoord.x;
    gl_FragColor = vec4(v_TexCoord.x,v_TexCoord.y,z,1.0);
}
//end