#pragma once
#include <glew.h>
#include <glfw3.h>
#include <string>

#include "Camera.h"


class SkyBox
{private:

	GLuint m_texture;
	GLuint m_VAO;
	GLuint m_program;

	Camera* m_camera;

	glm::mat4 m_matModel;
	glm::mat4 m_matPVM;

	float m_fogAmount;

public:
	SkyBox(Camera* _cam, GLuint _program, std::string _fileName, float _fogAmount);
	~SkyBox();

	void Update(float _deltaTime);
	void Render();



};

