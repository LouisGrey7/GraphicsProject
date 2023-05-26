#version 460 core

layout (points) in;
layout (triangle_strip, max_vertices = 24) out;

uniform mat4 PVM;

in VS_GS_VERTEX
{
    in vec4 Position;

} gs_in[];

out vec2 FragTexCoords;

void main()
{


    gl_Position = gs_in[0].Position + PVM * vec4(25.0, 0.0, 0.0, 0.0); // Top Right
    FragTexCoords = vec2(25.0, 0.0) / 100;
    EmitVertex();

    gl_Position = gs_in[0].Position + PVM * vec4(-25.0, 0.0, 0.0, 0.0); // Top Left
    FragTexCoords = vec2(-25.0, 0.0) / 100;
    EmitVertex();

    gl_Position = gs_in[0].Position + PVM * vec4(50.0, -50.0, 0.0, 0.0); // Bottom Right
    FragTexCoords = vec2(50.0, -50.0) / 100;
    EmitVertex();

    gl_Position = gs_in[0].Position + PVM * vec4(-50.0, -50.0, 0.0, 0.0); // Bottom Left
    FragTexCoords = vec2(-50.0, -50.0) / 100;
    EmitVertex();

    gl_Position = gs_in[0].Position + PVM * vec4(0.0, -80.0, 0.0, 0.0); // Bottom 
    FragTexCoords = vec2(0.0, -80.0) / 100;
    EmitVertex();

    gl_Position = gs_in[0].Position + PVM * vec4(-75.0, -125.0, 0.0, 0.0); // Bottom Left Point
    FragTexCoords = vec2(-75.0, -125.0) / 100;
    EmitVertex();

    gl_Position = gs_in[0].Position + PVM * vec4(0.0, -75.0, 0.0, 0.0); // Bottom 
    FragTexCoords = vec2(0.0, -75.0) / 100;
    EmitVertex();

    gl_Position = gs_in[0].Position + PVM * vec4(50.0, -50.0, 0.0, 0.0); // Bottom Right
    FragTexCoords = vec2(50.0, -50.0) / 100;
    EmitVertex();

    gl_Position = gs_in[0].Position + PVM * vec4(75.0, -125.0, 0.0, 0.0); // Bottom Right Point
    FragTexCoords = vec2(75.0, -125.0) / 100;
    EmitVertex();

    gl_Position = gs_in[0].Position + PVM * vec4(50.0, -50.0, 0.0, 0.0); // Bottom Right
    FragTexCoords = vec2(50.0, -50.0) / 100;
    EmitVertex();

    gl_Position = gs_in[0].Position + PVM * vec4(25.0, 0.0, 0.0, 0.0); // Top Right
    FragTexCoords = vec2(25.0, 0.0) / 100;
    EmitVertex();

    gl_Position = gs_in[0].Position + PVM * vec4(100.0, 0.0, 0.0, 0.0); // Top Right Point
    FragTexCoords = vec2(100.0, 0.0) / 100;
    EmitVertex();

    gl_Position = gs_in[0].Position + PVM * vec4(25.0, 0.0, 0.0, 0.0); // Top Right
    FragTexCoords = vec2(25.0, 0.0) / 100;
    EmitVertex();

    gl_Position = gs_in[0].Position + PVM * vec4(-25.0, 0.0, 0.0, 0.0); // Top Left
    FragTexCoords = vec2(-25.0, 0.0) / 100;
    EmitVertex();

    gl_Position = gs_in[0].Position + PVM * vec4(0.0, 50.0, 0.0, 0.0); // Top Point
    FragTexCoords = vec2(0.0, 50.0) / 100;
    EmitVertex();

    gl_Position = gs_in[0].Position + PVM * vec4(-25.0, 0.0, 0.0, 0.0); // Top Left
    FragTexCoords = vec2(-25.0, 0.0) / 100;
    EmitVertex();

    gl_Position = gs_in[0].Position + PVM * vec4(-50.0, -50.0, 0.0, 0.0); // Bottom Left
    FragTexCoords = vec2(-50.0, -50.0) / 100;
    EmitVertex();

    gl_Position = gs_in[0].Position + PVM * vec4(-100.0, 0.0, 0.0, 0.0); // Top Left Point
    FragTexCoords = vec2(-100.0, 0.0) / 100;
    EmitVertex();

    EndPrimitive();
}
