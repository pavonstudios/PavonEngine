#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
    gl_Position = proj * view * proj * vec4(vertexPosition_modelspace,1.0);
    
}