precision mediump float;
uniform sampler2D texture_sampler;               
varying vec2 v_TexCoord;													
void  main()                                        
{                                                            
	gl_FragColor = texture2D(texture_sampler,v_TexCoord) + vec4(0.0,0.3,0.2,0);
}
