#pragma once
#include <gtc/matrix_transform.hpp>

#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

struct VertexFormat
{
public:
	glm::vec3 Position;
	glm::vec2 TexCoord;
	glm::vec3 Normal;

	VertexFormat() {};

	VertexFormat(glm::vec3 Pos, glm::vec2 TexC, glm::vec3 Norm)
	{
		Position = Pos;
		TexCoord = TexC;
		Normal = Norm;
	}

};