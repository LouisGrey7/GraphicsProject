#pragma once

#include <vector>
#include <glm.hpp>

struct PointLight
{
	glm::vec3 position;
	glm::vec3 color;
	float ambientStrength;
	float specularStrength;

	float attenuationConstant;
	float attenuationLinear;
	float attenuationExponent;
};

class LightManager
{
public:
	
	LightManager();
	~LightManager();

	void CreatePointLight(int _index, glm::vec3 _position, glm::vec3 _color, float _ambientStrength, float _specularStrength, float _attConst, float _attLinear, float _attExp);

	static const int MAX_POINT_LIGHTS = 4;
	PointLight m_PointLights[MAX_POINT_LIGHTS];

};

