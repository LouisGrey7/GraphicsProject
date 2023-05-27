#pragma once
#include <gtc/matrix_transform.hpp>

#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

struct VertexPoint
{

public:
	glm::vec3 Position;

	VertexPoint() {};
	VertexPoint(glm::vec3 _pos)
	{
		Position = _pos;


	}

	bool operator==(const VertexPoint& other) const
	{
		return (Position == other.Position);
	}


};