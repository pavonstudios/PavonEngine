#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform vec4 mycolor;

void main()
{
    FragColor = texture(ourTexture, TexCoord);
}