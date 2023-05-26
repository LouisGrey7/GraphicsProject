
	
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
	uniform sampler2D alpha;
	uniform sampler2D grass;
	uniform sampler2D water;
	uniform sampler2D rock;
	uniform sampler2D sand;
	uniform float texscale;



	uniform vec3 CameraPos;	
	uniform float Shininess;
	uniform float FogAmount;
	uniform PointLight PointLights[MAX_POINT_LIGHTS];

	uniform float FogStart = 25.0f;
	uniform float FogEnd = 76.0f;
	uniform vec4 FogColor = vec4(0.4f, 0.4f, 0.4f, 1.0f);

	

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
		
		//Fog
		float Dist = distance(FragPos, CameraPos);
		
		float FogFactor = (Dist - FogStart) / FogEnd;

		FogFactor = clamp(FogFactor, 0.0f, FogAmount);

		// Calculate the final color
		vec4 TextureColor = vec4(LightOutput, 1.0f) * texture(ImageTexture0, FragTexCoords);

		FinalColor = mix(TextureColor, FogColor, FogFactor);
	}

	vec3 CalculateLight_Point(PointLight light)
	{
		float RimExponent = 3.0f;	
		vec3 RimColor = vec3(1.0f, 0.0f, 0.0f);
		
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

		//Rim Light Component
		float RimFactor = 1.0f - dot(Normal, ReverseViewDir);
		RimFactor = smoothstep(0.0f, 1.0f, RimFactor);
		RimFactor = pow(RimFactor, RimExponent);
		vec3 Rim = RimFactor * RimColor * light.Color;

		vec3 Light = vec3(Ambient + Diffuse + Specular + Rim);


		return Light;
	}



