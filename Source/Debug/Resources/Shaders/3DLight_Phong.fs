
	#version 460 core
	
	// Vertex Shader Inputs
	in vec2 FragTexCoords;
	in vec3 FragNormal;
	in vec3 FragPos;

	// Uniform Inputs
	uniform sampler2D ImageTexture0;
	uniform vec3 CameraPos;	
	uniform float AmbientStrength;
	uniform vec3 AmbientColor;
	uniform vec3 LightColor;
	uniform vec3 LightPos;		
	uniform float LightSpecularStength;
	uniform float Shininess;

	// Output
	out vec4 FinalColor;

	void main()
	{
		// Light Direction
		vec3 Normal = normalize(FragNormal);
		vec3 LightDir = normalize(FragPos - LightPos);

		// Ambient Component
		vec3 Ambient = AmbientStrength * AmbientColor;

		// Diffuse Component
		float DiffuseStrength = max(dot(Normal, -LightDir), 0.0f);
		vec3 Diffuse = DiffuseStrength * LightColor;

		// Specular Component
		vec3 ReverseViewDir = normalize(CameraPos - FragPos);
		vec3 ReflectedDir = reflect(LightDir, Normal);
		float SpecularReflectivity = pow(max(dot(ReverseViewDir, ReflectedDir), 0.0f), Shininess);
		vec3 Specular = LightSpecularStength * SpecularReflectivity * LightColor;

		// Combine the lighting components
		vec4 Light = vec4(Ambient + Diffuse + Specular, 1.0f);

		// Calculate the final color
		FinalColor = Light * texture(ImageTexture0, FragTexCoords);
	}


