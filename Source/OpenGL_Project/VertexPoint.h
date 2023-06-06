#pragma once
#include <gtc/matrix_transform.hpp>

#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

struct VertexPoint
{

public:
	glm::vec3 Position;
	glm::vec2 TextureCoords;


	VertexPoint() {};
	VertexPoint(glm::vec3 _pos)
	{
		Position = _pos;


	}
	VertexPoint(glm::vec3 _pos, glm:: vec2 _tex)
	{
		Position = _pos;
		TextureCoords = _tex;

	}

	bool operator==(const VertexPoint& other) const
	{
		return (Position == other.Position);
	}


};