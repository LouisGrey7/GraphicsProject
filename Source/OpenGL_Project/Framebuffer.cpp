#include "Framebuffer.h"
#include <stb_image.h>
#include <iostream>


Framebuffer::Framebuffer()
{

	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);


	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Utils::WindowWidth, Utils::WindowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureID, 0);

	GLuint RBO;
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Utils::WindowWidth, Utils::WindowHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{


		std::cout << "Error: Framebuffer failed to initialize correctly" << std::endl;

	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


}

Framebuffer::~Framebuffer()
{
}


GLuint Framebuffer::CreateTexture(std::string _fileName)
{
	GLuint textureID;
	std::string fullFilePath = "Resources/Textures/";
	fullFilePath.append(_fileName);

	// Load the Image data
	int imageWidth;
	int imageHeight;
	int imageComponents;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* imageData = stbi_load(fullFilePath.c_str(),
		&imageWidth, &imageHeight, &imageComponents, 0);

	// Create and bind a new texture template
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Check how many components the loaded image has (RGB or RGBA?)
	GLint loadedComponents = (imageComponents == 4) ? GL_RGBA : GL_RGB;

	// Populate the texture with the image data
	glTexImage2D(GL_TEXTURE_2D, 0, loadedComponents, imageWidth, imageHeight, 0,
		loadedComponents, GL_UNSIGNED_BYTE, imageData);

	// Setting the address mode for this texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Generate the mipmaps, free the memory and unbind the texture
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(imageData);
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}

void Framebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Framebuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


}

GLuint Framebuffer::GetTexture()
{
	return m_textureID;
}
