    #version 460 core

    // Vertex Shader Inputs
    in vec2 FragTexCoords;

    // Uniform Inputs
    uniform sampler2D ImageTexture;
    uniform sampler2D ImageTexture2;


    // Output
    out vec4 FinalColor;

    void main()
    {

	    vec3 uv = vec3(FragTexCoords.x / 800, FragTexCoords.y / 800, 1.0f);
	    float time = 1.0f;
	    vec3 raintex = texture(ImageTexture, vec2(uv.x * 2.0, uv.y * 0.1 + time * 0.125)).rgb / 8.0;
	    vec3 where = vec3(uv.x - raintex.x, uv.y - raintex.y, 1.0f);
	    vec3 texture = texture(ImageTexture, vec2(where.x, where.y)).rgb;


        FinalColor = vec4(texture, 1.0);
    }
