#pragma once
#include <glew.h>
#include <glfw3.h>
class Mesh
{
public:
	GLuint m_VAO;
	int m_indexCount;
	int m_drawType;
	bool m_arrayDraw;

	void Render();
	void Update();
	Mesh();
	~Mesh();

};

