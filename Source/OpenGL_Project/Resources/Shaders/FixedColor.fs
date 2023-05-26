	#version 460 core

	in vec2 FragTexCoords;
	in vec3 FragNormal;
	in vec3 FragPos;

	uniform vec3 ColorIn;

	out vec4 FinalColor;

	void main()
	{
		FinalColor = vec4(ColorIn, 1.0f);
	}


