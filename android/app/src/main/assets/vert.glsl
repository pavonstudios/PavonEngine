#version 100
attribute mediump vec3        position;
attribute mediump vec3			inColor;          
attribute mediump vec2          inUV; 

varying vec2 v_TexCoord;
varying vec3 color;       
uniform mat4          MVP;             
                
									
void main()                         
{                                   
	gl_Position = MVP * vec4(position,1.0);   
	v_TexCoord = vec2(inUV.x+2.0, inUV.y+2.0); 
	v_TexCoord += vec2(v_TexCoord.x + position.x, v_TexCoord.y + position.y);
	color = inColor;             
}                        