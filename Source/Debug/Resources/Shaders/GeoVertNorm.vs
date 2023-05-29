#version 460 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoords;
layout (location = 2) in vec3 Normal;

out VS_GS_VERTEX
{
	out vec4 Position;
	out vec2 FragTexCoords;
	out vec3 Normal;

} vs_out;

uniform mat4 PVM;
uniform mat4 Model;


void main()
{

	gl_Position = PVM * vec4(Position, 1.0f);


	vs_out.Position = gl_Position;
	vs_out.Normal = mat3(transpose(inverse(Model))) * Normal;
}