#version 100
precision highp float;
varying vec2 v_TexCoord;	
varying vec3 color;	
uniform sampler2D texture_sampler;               												
void  main()
{	
		gl_FragColor = vec4( texture2D(texture_sampler,v_TexCoord).rgb , 1.0 );
}//end