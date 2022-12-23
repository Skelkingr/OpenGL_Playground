#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 vertexColors;

out vec3 v_vertexColors;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.0);
	v_vertexColors = vertexColors;
}