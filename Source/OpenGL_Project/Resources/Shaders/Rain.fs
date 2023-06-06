    #version 460 core

    // Vertex Shader Inputs
    in vec2 FragTexCoords;

    // Uniform Inputs
    uniform sampler2D ImageTexture;
    uniform float Time;


    // Output
    out vec4 FinalColor;

    void main()
    {

	    vec2 uv = FragTexCoords.xy;
	    float time = Time;
	    vec3 raintex = texture(ImageTexture, vec2(uv.x * 2.0, uv.y * 0.1 + time * 0.125)).rgb / 8.0;
	    vec2 where = (uv.xy - raintex.xy);
	    vec3 texture = texture(ImageTexture, vec2(where.x, where.y)).rgb;


        FinalColor = vec4(texture, 1.0);
    }
