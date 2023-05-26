#pragma once
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>


#include <iostream>
#include <string>
#include <vector>
class DDDTexture
{
private:
	GLubyte* data3D;
	GLuint textureID;


	static const int textureWidth = 200;
	static const int textureHeight = 200;
	static const int textureDepth = 200;

	int texturePattern3D[textureWidth][textureHeight][textureDepth];


public:

	DDDTexture();
	~DDDTexture();


	void GeneratePattern3D();
	void GenerateData3D();
	void LoadTexture3D();


	GLuint GetTexture() { return textureID;};
};

