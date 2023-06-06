
	#version 460 core
	
	// Vertex Shader Inputs

	in vec2 FragTexCoords;

	// Uniform Inputs
	uniform sampler2D ImageTexture;

	// Output
	out vec4 FinalColor;

	void main()
	{
		
		FinalColor =  vec4(1.0f, 1.0f, 5.0f, 1.0f);//texture(ImageTexture, FragTexCoords);
	}