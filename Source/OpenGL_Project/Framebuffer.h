#pragma once
#include <glew.h>
#include <glfw3.h>
#include <string>
#include "Utility.h"


using namespace Utils;

class Framebuffer
{
private:

	GLuint FBO;
	GLuint m_textureID;

public:

	Framebuffer();
	~Framebuffer();

	GLuint CreateTexture(std::string _fileName);

	void Bind();
	void Unbind();
	GLuint GetTexture();

	



};

