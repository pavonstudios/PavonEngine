precision mediump float;
uniform sampler2D texture_sampler;               
varying vec2 UV;													
void  main()                                        
{                                                            
	gl_FragColor = texture2D(texture_sampler,UV) + vec4(0.0,0.3,0.2,0);
}
