#version 460 core

layout (points) in;
layout (triangle_strip, max_vertices = 3) out;

uniform mat4 PVM;

in VS_GS_VERTEX
{

	in vec4 Position;

} gs_in[];

void main()
{
	gl_Position = gs_in[0].Position + PVM * vec4(0.0f, 0.5f, 0.0f, 1.0f);
	EmitVertex();
	gl_Position = gs_in[0].Position + PVM * vec4(-0.45f, -0.30f, 0.0f, 1.0f);
	EmitVertex();
	gl_Position = gs_in[0].Position + PVM * vec4(0.45f, -0.30f, 0.0f, 1.0f);
	EmitVertex();

	EndPrimitive();
}