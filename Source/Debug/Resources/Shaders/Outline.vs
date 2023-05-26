#version 460 core

layout (location = 0) in vec3 Position;


uniform mat4 camMatrix;
uniform mat4 model;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;
uniform float outlining;


void main()
{
	vec3 currentPos = vec3(model * translation * rotation * scale * outlining * vec4(Position, 1.0f));
	gl_Position = camMatrix * vec4(currentPos, 1.0);
	
}