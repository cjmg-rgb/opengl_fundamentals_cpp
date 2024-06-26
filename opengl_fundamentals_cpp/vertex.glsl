#version 440

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;
layout (location = 2) in vec3 vertex_normal;
layout (location = 3) in vec2 vertex_texcoord;

out vec3 fs_position;
out vec3 fs_color;
out vec3 fs_normal;
out vec2 fs_texcoord;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main()
{
	fs_position = (ModelMatrix * vec4(vertex_position, 1.f)).xyz;
	fs_color    = vertex_color;
	fs_normal   = mat3(ModelMatrix) * vertex_normal;
	fs_texcoord = vec2(vertex_texcoord.x, vertex_texcoord.y * -1.f);

	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(vertex_position, 1.f);
}