#pragma once

#include <glew.h>
#include <glfw3.h>

#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>
#include "Mesh.h"


class Sphere : public Mesh
{
public:
	Sphere(float _radius, int _fidelity);
	~Sphere();
};

