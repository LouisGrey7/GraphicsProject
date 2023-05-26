#pragma once
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>


#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <time.h>
#include <math.h>


class PerlinNoise
{
private:





public:

	double RandomValue(int x, int y);

	double Smooth(int x, int y);

	double LinearInterpolate(double point1, double point2, double Fract);

	double CosineInterpolate(double point1, double point2, double Fract);

	double SmoothInterpolate(double x, double y);

	double TotalNoisePerPoint(int x, int y);

	void GenerateNoise();
};

