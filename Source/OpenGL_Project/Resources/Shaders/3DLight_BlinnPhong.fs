
	#version 460 core
	#define MAX_POINT_LIGHTS 4



	// Vertex Shader Inputs
	in vec2 FragTexCoords;
	in vec3 FragNormal;
	in vec3 FragPos;

	//Light Struct
	struct PointLight
	{
		vec3 Position;
		vec3 Color;
		float AmbientStrength;
		float SpecularStrength;
		float AttenuationConstant;
		float AttenuationLinear;
		float AttenuationExponent;
	}; 

	// Uniform Inputs
	uniform sampler2D ImageTexture0;
	uniform vec3 CameraPos;	
	uniform float Shininess;
	uniform PointLight PointLights[MAX_POINT_LIGHTS];

	// Output
	out vec4 FinalColor;

	

	
	vec3 CalculateLight_Point(PointLight light);

	void main()
	{
		vec3 LightOutput = vec3(0.0f, 0.0f, 0.0f);

		for(int i = 0; i < MAX_POINT_LIGHTS; i++)
		{
			LightOutput += CalculateLight_Point(PointLights[i]);
		}
		// Calculate the final color
		FinalColor =  vec4(LightOutput, 1.0f) * texture(ImageTexture0, FragTexCoords);
	}

	vec3 CalculateLight_Point(PointLight light)
	{
		
	
		vec3 Normal;
		Normal = normalize(FragNormal);
		vec3 LightDir = normalize(FragPos - light.Position);

		vec3 Ambient = light.AmbientStrength * light.Color;

		float DiffuseStrength = max(dot(Normal, -LightDir), 0.0f);
		vec3 Diffuse = DiffuseStrength * light.Color;


		vec3 ReverseViewDir = normalize(CameraPos - FragPos);
		
		vec3 HalfwayVector = normalize(-LightDir + ReverseViewDir);
		float SpecularReflectivity = pow(max(dot(Normal, HalfwayVector), 0.0f), Shininess);
		
		vec3 Specular = light.SpecularStrength * SpecularReflectivity * light.Color;

		vec3 Light = vec3(Ambient + Diffuse + Specular);


		return Light;
	}
