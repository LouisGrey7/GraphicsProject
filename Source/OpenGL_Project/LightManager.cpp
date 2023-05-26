#include "LightManager.h"

LightManager::LightManager()
{


}

LightManager::~LightManager()
{
}

void LightManager::CreatePointLight(int _index, glm::vec3 _position, glm::vec3 _color, float _ambientStrength, float _specularStrength, float _attConst, float _attLinear, float _attExp)
{
	PointLight light;
	light.position = _position;
	light.color = _color;
	light.ambientStrength = _ambientStrength;
	light.specularStrength = _specularStrength;
	light.attenuationConstant = _attConst;
	light.attenuationLinear = _attLinear;
	light.attenuationExponent = _attExp;
	

	m_PointLights[_index] = light;

}


