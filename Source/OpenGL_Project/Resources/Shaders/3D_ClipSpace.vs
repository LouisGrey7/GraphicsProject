
#version 460 core

// Vertex data interpretation
layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoords;

// Inputs
uniform mat4 PVM;

// Outputs to Fragment Shader
// out vec2 FragTexCoords;

void main()
{
	gl_Position = PVM * vec4(Position, 1.0f);
	// FragTexCoords = TexCoords;
}


