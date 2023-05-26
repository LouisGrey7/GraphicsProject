#include "Quad.h"

Quad::Quad()
{

	float vertices[] =
	{
		//   Coordinates		//Tex			//Normals
		-10.5f, 10.5f, 10.0f,		0.0f, 1.0f,		0.0f, 0.0f, 1.0f,	 //1
		-10.5f,-10.5f, 10.0f,		0.0f, 0.0f,		0.0f, 0.0f, 1.0f,	//2
		 10.5f, -10.5f, 10.0f,		1.0f, 0.0f,		0.0f, 0.0f, 1.0f,	 //3
		 10.5f, 10.5f, 10.0f,		1.0f, 1.0f,		0.0f, 0.0f, 1.0f	//4
	};	 


	unsigned int indices[] =
	{
		// Right
		0, 1, 2,
		0, 2, 3
	};

	//Generate, bind and fill buffers
	unsigned int VBO, EBO;
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 4 * 7 * sizeof(float), &vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), &indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	m_indexCount = 6;
	m_drawType = GL_TRIANGLES;
	m_arrayDraw = false;

}

Quad::~Quad()
{
}
