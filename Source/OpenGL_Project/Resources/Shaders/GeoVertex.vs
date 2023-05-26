#version 460 core

layout (location = 0) in vec3 Position;


out VS_GS_VERTEX
{
	out vec4 Position;


} vs_out;

uniform mat4 PVM;

void main()
{

	gl_Position = PVM * vec4(Position, 1.0f);


	vs_out.Position = gl_Position;

}