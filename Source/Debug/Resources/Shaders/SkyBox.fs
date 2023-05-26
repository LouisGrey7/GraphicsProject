#version 460 core

//Vertex Shader Inputs
in vec3 FragTexCoords;
in vec3 FragPos;



//Uniform Inputs
uniform samplerCube Texture0;
uniform float FogAmount;
uniform vec3 CameraPos;	



uniform float FogStart = 25.0f;
uniform float FogEnd = 76.0f;
uniform vec4 FogColor = vec4(0.4f, 0.4f, 0.4f, 1.0f);

//Output
out vec4 FinalColor;


void main()
{
	//Fog
	float Dist = distance(FragPos, CameraPos);
	
	float FogFactor = (Dist - FogStart) / FogEnd;
	
	FogFactor = clamp(FogFactor, 0.0f, FogAmount);

	vec4 TextureColor = texture(Texture0, FragTexCoords);

	FinalColor = mix(TextureColor, FogColor, FogFactor);

}


