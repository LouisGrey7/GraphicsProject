    #version 460 core

    // Vertex Shader Inputs
    in vec2 FragTexCoords;

    // Uniform Inputs
    uniform sampler2D ImageTexture;

    // Output
    out vec4 FinalColor;

    void main()
    {
        vec3 Color = texture2D(ImageTexture, FragTexCoords).rgb;
        float Greyscale = (Color.r * 0.3f, Color.g * 0.11, Color.b * 0.59f);
        FinalColor = vec4(vec3(Greyscale), 1.0f);
    }