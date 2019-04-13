
attribute vec3        position;       
attribute vec2          inUV;         
uniform mat4          MVP;             
varying vec2 UV;                   
									
void main()                         
{                                   
	gl_Position = MVP * vec4(position,1.0);   
	UV = inUV;              
}                        