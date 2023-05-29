    #version 460 core

    // Vertex Shader Inputs
    in vec2 FragTexCoords;

    // Uniform Inputs
    uniform sampler2D ImageTexture;

    // Output
    out vec4 LastColor;

    void main()
    {
        vec4 FinalColor = texture(ImageTexture, FragTexCoords);
        LastColor = vec4(1 - FinalColor.x, 1- FinalColor.y, 1 - FinalColor.z, 1.0f);
    }