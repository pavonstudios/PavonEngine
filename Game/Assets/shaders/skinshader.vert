#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(set = 1, binding = 0) uniform UBO_Node{
	mat4 matrix;
	mat4 joint_matrix[128];
	float joint_count;
}node;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec4 inJoint0;
layout(location = 4) in vec4 inWeight0;

layout(location = 0) out vec3 outWorldPos;
layout(location = 1) out vec2 fragTexCoord;

void main() {
	vec4 local_position;
	mat4 skin_matrix;
	
	skin_matrix =
		inWeight0.x * node.joint_matrix[int(inJoint0.x)] +
		inWeight0.y * node.joint_matrix[int(inJoint0.y)] +
		inWeight0.z * node.joint_matrix[int(inJoint0.z)] +
		inWeight0.w * node.joint_matrix[int(inJoint0.w)];
	
    mat4 mv = ubo.view * ubo.model;

	vec4 pos = mv * skin_matrix * vec4(inPosition,1.0);

	gl_Position = ubo.proj * pos;

    outWorldPos = inColor;
    fragTexCoord = inTexCoord;
}