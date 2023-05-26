
	#version 460 core

	// Vertex data interpretation
	layout (location = 0) in vec3 Position;


	// Inputs
	uniform mat4 PV;
	uniform mat4 Model;


	// Outputs to Fragment Shader
	out vec3 FragTexCoords;
	out vec3 FragPos;

	void main()
	{
		vec4 pos = PV * vec4(Position, 1.0f);
		gl_Position = vec4(pos.x,pos.y,pos.w, pos.w);
		FragTexCoords = vec3(Position.x, Position.y, -Position.z);

		FragPos = vec3(Model * vec4(Position, 1.0f));

	}


