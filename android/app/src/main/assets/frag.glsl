#version 100
precision mediump float;
varying vec2 v_TexCoord;	
varying vec3 color;	
uniform sampler2D texture_sampler;               
												
void  main()                                        
{                                                            
	gl_FragColor = texture2D(texture_sampler,v_TexCoord) + vec4(color,1.0) + vec4(0.0,0.3,0.2,0);
}
