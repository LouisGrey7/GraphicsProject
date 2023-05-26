#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Mesh.h"

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


class PointMesh : public Mesh
{
private:


public:

	PointMesh();
	~PointMesh();


};

