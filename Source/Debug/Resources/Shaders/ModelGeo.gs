#version 460 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

uniform mat4 PVM;

in VS_GS_VERTEX
{
    in vec4 Position;

} gs_in[];

out vec2 FragTexCoords;

void main()
{

    gl_Position = gs_in[0].Position
    EmitVertex();

    vec3 NormalizedNormal = normalize(gs_in(0).Normal);
    gl_Position = gs_in[0].Position + PVM * vec4(NormalizedNormal, 0.0f) * Magnitude;
    EmitVertex();


    EndPrimitive();
}
