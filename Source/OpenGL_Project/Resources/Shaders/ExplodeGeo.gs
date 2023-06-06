#version 460 core

layout (triangles) in;
//layout (line_strip, max_vertices = 6) out;
layout (triangle_strip, max_vertices = 3) out;


uniform mat4 PVM;
uniform float Time;


in VS_GS_VERTEX
{
    in vec4 Position;
	in vec2 FragTexCoords;
    in vec3 Normal;


} gs_in[];

out vec2 FragTexCoords;

vec3 GetNormal()
{
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   return normalize(cross(a, b));
} 

vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 2.0;
    vec3 direction = normal * ((sin(Time) + 1.0) / 2.0) * magnitude; 
    return position + vec4(direction, 0.0);
}


void main()
{
    vec3 normal = GetNormal();

    gl_Position = explode(gs_in[0].Position, normal);
    FragTexCoords = gs_in[0].FragTexCoords;
    EmitVertex();

    gl_Position = explode(gs_in[1].Position, normal);
    FragTexCoords = gs_in[1].FragTexCoords;
    EmitVertex();

    gl_Position = explode(gs_in[2].Position, normal);
    FragTexCoords = gs_in[0].FragTexCoords;
    EmitVertex();

    //FragTexCoords = gs_in[0].FragTexCoords;
    //EmitVertex();

    vec3 NormalizedNormal = normalize(gs_in[0].Normal);
    gl_Position = gs_in[0].Position + PVM * vec4(NormalizedNormal, 0.0f) * 5.0f;
    EmitVertex();


    EndPrimitive();

}
